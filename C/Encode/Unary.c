// unary code: 一元编码，一进制编码，N=N个1后面加个0
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc != 2 || isdigit(argv[1][0]) == 0)
	{
		printf("Usage example: %s 3\n", argv[0]);
		return -1;
	}

	int num = atoi(argv[1]);
	if (num > 64)
	{
		printf("Sorry, digit can't bigger than 64\n");
		return -1;
	}

	for (int i = 0; i < num; ++i)
	{
		putchar('1');
	}
	puts("0");

	return 0;
}

