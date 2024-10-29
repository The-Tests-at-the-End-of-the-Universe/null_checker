
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
	test2[0] = 0;
}
void new_test()
{
	char *test3;
	test3 = malloc(20);
	test3[0] = 1;
}

int main ()
{
	printf("here in program\n");
	new_test();
	malloc_test();
	return (0);
}