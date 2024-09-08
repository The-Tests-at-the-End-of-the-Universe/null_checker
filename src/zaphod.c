/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   zaphod.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/30 15:42:45 by spenning      #+#    #+#                 */
/*   Updated: 2024/09/08 21:17:37 by mynodeus      ########   odam.nl         */
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


void print_backtrace(void)
{
	int nptrs;
	void *buffer[BT_BUF_SIZE];
	char **strings;

	nptrs = backtrace(buffer, BT_BUF_SIZE);
	fprintf(stderr, "backtrace() returned %d addresses\n", nptrs);
	/* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
			would produce similar output to the following: */
	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL)
	{
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}
	for (int j = 0; j < nptrs; j++)
	{
		fprintf(stderr, "%s\n", strings[j]);
		// free(strings);
	}
}

// Overriding malloc
void *malloc(size_t size)
{
	void		*ret;
	static int	internal_malloc = 0;
	// Dl_info info_dl;

	if (real_malloc == NULL)
	{
		const char *error_msg = "malloc called before initialization\n";
		write(STDERR_FILENO, error_msg, sizeof(error_msg));
		return NULL;
	}
	if (internal_malloc == 0)
	{
		internal_malloc = 1;
		data_ptr->malloc_count++;
		lstadd(data_ptr);
		backtrace_full(state, 0, full_callback, error_callback, NULL);
		write(1, "\n", 1);
		print_backtrace();
		// ret = __builtin_return_address(0);
		// fprintf (stderr, "Caller name: %p\n", ret);
		// if(dladdr(ret, &info_dl))
		// 	fprintf( stderr, "symbol name: %s\n", info_dl.dli_sname);
		write(STDOUT_FILENO, "malloc\n", 7);
		internal_malloc = 0;
	}
	ret = real_malloc(size);
	return (ret); // Call the original malloc
}



int main_hook(int argc, char **argv, char **envp)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	data_ptr = &data;
	state = backtrace_create_state(NULL, 0, error_callback, NULL);
	printf("--- Before main ---\n");
	int ret = main_orig(argc, argv, envp);
	printf("--- After main ----\n");
	lstprint(data.mallocs);
	printf("main() returned %d\n", ret);
	return ret;
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
	return orig(main_hook, argc, argv, init, fini, rtld_fini, stack_end);
}