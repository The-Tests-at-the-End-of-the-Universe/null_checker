
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


int main ()
{
	printf("here in program\n");
	malloc_test();
	return (0);
}