/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   zaphod.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/30 15:42:45 by spenning      #+#    #+#                 */
/*   Updated: 2024/10/28 17:43:43 by mynodeus      ########   odam.nl         */
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

// Function pointer to hold the original malloc
static void *(*real_malloc)(size_t) = NULL;
static int (*main_orig)(int, char **, char **);
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static struct s_data * data_ptr = NULL;
struct backtrace_state *state;

#define BT_BUF_SIZE 100


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
	}
	pthread_mutex_unlock(&lock);
}


// Finalization code
void __attribute__((destructor)) finalize() 
{
	pthread_mutex_lock(&lock);
	write (STDOUT_FILENO, "finalizing\n", 11);
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
	(void)data;

	if (pathname != NULL || function != NULL || line_number != 0)
	{
		if (unknown_count)
		{
			fprintf(stderr, "    ...\n");
			unknown_count = 0;
		}
		const char *filename = rindex(pathname, '/');
		if (filename)
			filename++;
		else
			filename = pathname;
		fprintf(stderr, "  %s:%d -- %s\n", filename, line_number, function);
	}
	else
		unknown_count++;
  return 0;
};


void add_backtrace(t_mallocs *node)
{
	int nptrs;
	void *buffer[BT_BUF_SIZE];
	char **strings;

	nptrs = backtrace(buffer, BT_BUF_SIZE);
	/* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
			would produce similar output to the following: */
	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL)
	{
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}
	node->backtrace = strings;
	node->nptrs=nptrs;
}

void	print_backtrace(t_mallocs *node)
{
	for (int j = 0; j < node->nptrs; j++)
	{
		fprintf(stderr, "%s\n", node->backtrace[j]);
	}
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
			dprintf(2 ,"iteration %d in child %d nullcheck_count %d \n", i, getpid(), data_ptr->null_check_count);
			if (temp->fail == 1 && data_ptr->null_check_count == temp->num)
			{
				dprintf(2 ,"here in malloc %d in child %d\n", temp->num, getpid());
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
		backtrace_full(state, 0, full_callback, error_callback, NULL);
		if(lstadd(data_ptr))
		{
			perror("lstadd");
			exit(1);
		}
		data_ptr->malloc_count++;
		internal_malloc = 0;
	}
	ret = real_malloc(size);
	return (ret); // Call the original malloc
}

int main_hook_count(t_data *data, int argc, char **argv, char **envp)
{
	state = backtrace_create_state(NULL, 0, error_callback, NULL);
	printf("--- Before main ---\n");
	int ret = main_orig(argc, argv, envp);
	printf("--- After main ----\n");
	lstprint(data->mallocs);
	printf("number one main() returned %d\n", ret);
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
		dprintf(2, "pid %d\n", getpid());
		ret = main_orig(argc, argv, envp);
		printf("number %d main() returned %d\n", data.num , ret);
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

	i = 0;
	temp = data_ptr->mallocs;
	childs = real_malloc(sizeof(childs)* count);
	if (childs == NULL)
	{
		perror("null_check childs");
		exit(EXIT_FAILURE);
	}
	bzero(childs, sizeof(childs)* count);
	printf("create childs\n");
	printf("count: %d\n", count);
	while (i < count)
	{
		dprintf(2, "child number %d\n", i);
		temp->fail = 1;
		fork_tests(&childs[i], *temp, argc, argv, envp);
		temp->fail = 0;
		temp = temp->next;
		i++;
	}
	i = 0;
	while (i < count)
	{
		if (wait_child(childs[i]))
		{
			printf("SEGFAULTTTT in %d\n", i);
			print_backtrace(lstgive_node(data_ptr, i));
		}
		data_ptr->exit_code = 2;
		i++;
	}
}




int main_hook(int argc, char **argv, char **envp)
{
	int ret;
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	data_ptr = &data;
	ret = main_hook_count(&data, argc, argv, envp);
	data.null_check = 1;
	printf("going into null_check\n");
	if (data.null_check)
		main_hook_null_check(ret, argc, argv, envp);
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