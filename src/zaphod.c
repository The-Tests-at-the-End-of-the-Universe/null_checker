/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   zaphod.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/30 15:42:45 by spenning      #+#    #+#                 */
/*   Updated: 2024/10/29 22:22:10 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <dlfcn.h>
#include <zaphod.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <execinfo.h>
#include <backtrace.h>
#include <stdarg.h>

// acknowledgement https://www.vishalchovatiya.com/hack-c-cpp-application-using-rtld-next-with-an-easy-example/
// compilation 
// gcc -fPIC -shared -o malloc.so malloc.c -ldl -pthread && LD_PRELOAD=./malloc.so your_application

// acknowledgement https://gist.github.com/apsun/1e144bf7639b22ff0097171fa0f8c6b1

// acknowledgement https://tjysdsg.github.io/libbacktrace/
// acknowledgement https://stackoverflow.com/questions/11492149/passing-arguments-to-a-library-loaded-with-ld-preload

// Function pointer to hold the original malloc
static void *(*real_malloc)(size_t) = NULL;
static int (*main_orig)(int, char **, char **);
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static struct s_data * data_ptr = NULL;
struct backtrace_state *state;
static int	debug_flag = 0;
static int	print_bt = 0;
char *ignore_funcs = NULL;

#define BT_BUF_SIZE 100


void	debug(char *format, ...)
{
	va_list	ptr;

	if (debug_flag == 1)
	{
		va_start(ptr, format);
		vdprintf(2, format, ptr);
		va_end(ptr);
	}
}


// Constructor function to initialize real_malloc
void __attribute__((constructor)) init_malloc() 
{
	pthread_mutex_lock(&lock);
	if (real_malloc == NULL)
	{
		real_malloc = (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc");
		if (real_malloc == NULL)
		{
			const char *error_msg = "Error in dlsym for malloc\n";
			write(STDERR_FILENO, error_msg, sizeof(error_msg));
		}
		if(getenv("DEBUG"))
			debug_flag = 1;
		if(getenv("PRINT_BT"))
			print_bt = 1;
		ignore_funcs = getenv("IGNORE");
	}
	pthread_mutex_unlock(&lock);
}


// Finalization code
void __attribute__((destructor)) finalize() 
{
	pthread_mutex_lock(&lock);
	if (debug_flag)
		write (STDOUT_FILENO, RED "destructor\n" RESET, 16);
	pthread_mutex_unlock(&lock);
}

/*

  A libbacktrace example program.

  libbacktrace is found at https://github.com/ianlancetaylor/libbacktrace

  Compile this file with: gcc -g -o test test.c -lbacktrace

*/

static void error_callback (void *data, const char *message, int error_number)
{
	(void)data;

	if (error_number == -1) 
	{
		fprintf(stderr, "If you want backtraces, you have to compile with -g\n\n");
		_Exit(1);
	} 
	else 
		fprintf(stderr, "Backtrace error %d: %s\n", error_number, message);
};

static int full_callback (void *data, uintptr_t pc, const char *pathname, int line_number, const char *function) 
{
	(void)pc;
	static int unknown_count = 0;
	t_mallocs *temp;
	char *new;
	char sprintf_buf[BT_BUF_SIZE];
	int		len;
	(void)data;

	if (pathname != NULL || function != NULL || line_number != 0)
	{
		if (unknown_count)
		{
			// fprintf(stderr, "    ...\n");
			unknown_count = 0;
		}
		const char *filename = rindex(pathname, '/');
		if (filename)
			filename++;
		else
			filename = pathname;
		if (data_ptr->mallocs)
			temp = lstlast(data_ptr->mallocs);
		if (temp)
		{
			len = sprintf(sprintf_buf, "  %s:%d -- %s\n", filename, line_number, function);
			new = calloc(1, len);
			if (new == NULL)
			{
				perror("full_callback");
				exit(EXIT_FAILURE);
			}
			sprintf(new, "  %s:%d -- %s\n", filename, line_number, function);
			temp->backtrace = array_add(temp->backtrace, new);
		}
	}
	else
		unknown_count++;
  return 0;
};


void	print_backtrace(t_mallocs *node)
{
	fprintf(stderr, RED "malloc #%d unprotected\n" RESET, node->num);
	for (int j = 1; node->backtrace[j] != NULL; j++)
	{
		if (print_bt)
			fprintf(stderr, MAG "      %s" RESET, node->backtrace[j]);
	}
}

int	check_backtrace()
{
	t_mallocs *temp;
	char	*token;
	char	*trim;
	char	*str_dup;
	int		token_count;
	int		i;

	i = 0;
	token_count = 1;
	if (!data_ptr->mallocs)
		return (0);
	temp = lstlast(data_ptr->mallocs);
	temp->malloc_calling_func = strdup(strrchr(temp->backtrace[1], ' '));
	temp->malloc_calling_func++;
	trim = strchr(temp->malloc_calling_func, '\n');
	trim[0] = 0;
	if (!ignore_funcs)
		return (0);
	str_dup = strdup(ignore_funcs);
	if (str_dup == NULL)
	{
		perror("str_dup in check_backtrace");
		exit (1);
	}
	while (str_dup[i])
	{
		if (str_dup[i] == ',')
			token_count++;
		i++;
	}
	i = 0;
	while (i < token_count)
	{
		token = __strtok_r(str_dup, ",", &str_dup);
		debug(BLU "tokencount: %d token:%s func:%s\n" RESET, token_count, token, temp->malloc_calling_func);
		if (!strncmp(token, temp->malloc_calling_func, strlen(temp->malloc_calling_func)))
		{
			debug(YEL "func: %s ignored\n" RESET, temp->malloc_calling_func);
			return (1);
		}
		i++;
	}
	return (0);
}

// Overriding malloc
// take out GI_UI_FILE_DOALLOCATE malloc
void *malloc(size_t size)
{
	void		*ret;
	static int	internal_malloc = 0;
	int i;

	i = 0;
	t_mallocs *temp;
	if (data_ptr->null_check == 1)
	{
		temp = data_ptr->mallocs;
		data_ptr->null_check_count++;
		while (temp)
		{
			// debug(BLU"\titeration %d in child %d nullcheck_count %d \n" RESET, i, getpid(), data_ptr->null_check_count);
			if (temp->fail == 1 && data_ptr->null_check_count == temp->num)
			{
				// debug(BLU"\there in malloc %d in child %d\n" RESET, temp->num, getpid());
				return (NULL);
			}
			i++;
			temp = temp->next;
		}
		ret = real_malloc(size);
		return (ret);
	}
	if (real_malloc == NULL)
	{
		const char *error_msg = "malloc called before initialization\n";
		write(STDERR_FILENO, error_msg, sizeof(error_msg));
		return NULL;
	}
	if (internal_malloc == 0)
	{
		internal_malloc = 1;
		if(lstadd(data_ptr))
		{
			perror("lstadd");
			exit(1);
		}
		backtrace_full(state, 0, full_callback, error_callback, NULL);
		if (check_backtrace())
		{
			lstdelone(lstlast(data_ptr->mallocs));
			t_mallocs *second_last = lst_second_last(data_ptr->mallocs);
			second_last->next = NULL;
		}
		else
			data_ptr->malloc_count++;
		internal_malloc = 0;
	}
	ret = real_malloc(size);
	return (ret); // Call the original malloc
}

int main_hook_count(t_data *data, int argc, char **argv, char **envp)
{
	state = backtrace_create_state(NULL, 0, error_callback, NULL);
	debug(BLU "--- Before main ---\n" RESET);
	int ret = main_orig(argc, argv, envp);
	debug(BLU "--- After main ----\n\n" RESET);
	debug(GRN "first main() returned:\t%d\n" RESET, ret);
	debug(GRN "malloc count:\t\t%d\n\n" RESET, data->malloc_count);
	return (data->malloc_count);
}

void	fork_tests(pid_t *child, t_mallocs data, int argc, char **argv, char **envp)
{
	int	ret;
	*child = fork();
	if (*child == -1)
		exit(1);
	if (*child == 0)
	{
		debug(BCYN"\tpid %d\n"RESET, getpid());
		ret = main_orig(argc, argv, envp);
		debug(BCYN"\tnumber %d main() returned %d\n"RESET, data.num , ret);
		exit(0);
	}
}


int wait_child(int pid)
{
	int got_pid;
	int status;

	got_pid = waitpid(pid, &status, 0);
	if ((got_pid == -1) && (errno != EINTR))
	{
		perror("waitpid");
		exit(1);
	}
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGSEGV)
			return (1);
	}
	return (0);
}

void main_hook_null_check(int count, int argc, char **argv, char **envp)
{
	int i;
	t_mallocs *temp;
	pid_t	*childs;
	int devnull;
	int stdout_int;
	
	devnull = open("/dev/null", O_WRONLY);
	if (devnull == -1)
	{
		perror("open in null_check");
		exit(EXIT_FAILURE);
	}
	i = 0;
	temp = data_ptr->mallocs;
	childs = real_malloc(sizeof(childs)* count);
	if (childs == NULL)
	{
		perror("null_check childs");
		exit(EXIT_FAILURE);
	}
	bzero(childs, sizeof(childs)* count);
	debug(BMAG "\tcreating childs\n" RESET);
	stdout_int = dup(STDOUT_FILENO);
	if (stdout_int == -1)
	{
		perror("stdout_int in null_check");
		exit(EXIT_FAILURE);
	}
	if (dup2(devnull, STDOUT_FILENO) == -1)
	{
		perror("dev/null in null_check");
		exit(EXIT_FAILURE);
	}
	while (i < count)
	{
		debug(MAG "\tchild number %d\n" RESET, i);
		temp->fail = 1;
		fork_tests(&childs[i], *temp, argc, argv, envp);
		temp->fail = 0;
		temp = temp->next;
		i++;
	}
	if (dup2(stdout_int, STDOUT_FILENO) == -1)
	{
		perror("restore stdout in null_check");
		exit(EXIT_FAILURE);
	}
	if (close(stdout_int) == -1)
	{
		perror("close stdout_int in null_check");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < count)
	{
		if (wait_child(childs[i]))
		{
			print_backtrace(lstgive_node(data_ptr, i));
			data_ptr->exit_code = 2;
		}
		i++;
	}
}

int main_hook(int argc, char **argv, char **envp)
{
	int ret;
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	data_ptr = &data;
	debug(RED "DEBUG MODE\n\n" RESET);
	ret = main_hook_count(&data, argc, argv, envp);
	data.null_check = 1;
	if (data.null_check)
	{
		debug(YEL "-- start null_check mode --\n" RESET);
		main_hook_null_check(ret, argc, argv, envp);
		debug(YEL "--- end null_check mode ---\n" RESET);
	}
	return (data_ptr->exit_code);
}

int __libc_start_main(
	int (*main)(int, char **, char **),
	int argc,
	char **argv,
	int (*init)(int, char **, char **),
	void (*fini)(void),
	void (*rtld_fini)(void),
	void *stack_end)
{
	/* Save the real main function address */
	main_orig = main;

	/* Find the real __libc_start_main()... */
	typeof(&__libc_start_main) orig = dlsym(RTLD_NEXT, "__libc_start_main");

	/* ... and call it with our custom main function */
	return (orig(main_hook, argc, argv, init, fini, rtld_fini, stack_end));
}