#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <pthread.h>
#include <unistd.h>

// acknowledgement https://www.vishalchovatiya.com/hack-c-cpp-application-using-rtld-next-with-an-easy-example/
// compilation 
// gcc -fPIC -shared -o malloc.so malloc.c -ldl -pthread && LD_PRELOAD=./malloc.so your_application

// acknowledgement https://gist.github.com/apsun/1e144bf7639b22ff0097171fa0f8c6b1

// Function pointer to hold the original malloc
static void *(*real_malloc)(size_t) = NULL;
static int (*main_orig)(int, char **, char **);
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Constructor function to initialize real_malloc
__attribute__((constructor))
void init_malloc() {
	pthread_mutex_lock(&lock);
	if (real_malloc == NULL) {
		real_malloc = (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc");
		if (real_malloc == NULL) {
			const char *error_msg = "Error in dlsym for malloc\n";
			write(STDERR_FILENO, error_msg, sizeof(error_msg));
		}
	}
	pthread_mutex_unlock(&lock);
}


// Overriding malloc
void *malloc(size_t size)
{
	void *ret;
	if (real_malloc == NULL) {
		const char *error_msg = "malloc called before initialization\n";
		write(STDERR_FILENO, error_msg, sizeof(error_msg));
		return NULL;
	}
	write(STDOUT_FILENO, "malloc\n", 7);
	ret = real_malloc(size);
	return (ret); // Call the original malloc
}

// Finalization code
void __attribute__((destructor)) finalize() {
	pthread_mutex_lock(&lock);
	write (STDOUT_FILENO, "finalizing\n", 11);
	pthread_mutex_unlock(&lock);
}

int main_hook(int argc, char **argv, char **envp)
{
	printf("--- Before main ---\n");
	int ret = main_orig(argc, argv, envp);
	printf("--- After main ----\n");
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