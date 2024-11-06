
#include <stdlib.h>
#include <stdio.h>

void malloc_test()
{
	char *test;
	char *test2;
	test = malloc(10);
	test2 = malloc(20);
	test2 = malloc(20);
	test2 = malloc(20);
	test2 = malloc(20);
	test2 = malloc(20);
	test2 = malloc(20);
	test2[0] = 0;
}
void new_test()
{
	char *test3;
	test3 = malloc(20);
	// test3 = NULL;
	test3[0] = 1;
}

int main (int argc, char **argv)
{
	printf("here in program argc:%d\n", argc);
	new_test();
	malloc_test();
	// exit (1);
	return (0);
}