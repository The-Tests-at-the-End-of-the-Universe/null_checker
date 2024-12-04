
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
	// test2 = malloc(20);
	// test2 = malloc(20);
	// test2 = malloc(20);
}
void new_test()
{
	char *test3;
	test3 = malloc(20);
	for (int i = 0; i < 90; i++)
	{
		malloc(100);
	}
	
}

int main (int argc, char **argv)
{
	printf("here in program argc:%d\n", argc);
	new_test();
	malloc_test();
	printf("here\n");
	return (0);
}