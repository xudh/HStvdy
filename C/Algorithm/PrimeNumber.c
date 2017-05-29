// 输出N以内的素数或质数
#include <stdio.h>
#include <math.h>

int main(void)
{
	puts("PrimeNumber: Input prime number isn't more than it");
	int N = 0;
	if (scanf("%d", &N) != 1 || N < 2)
		return 0;

	int maxId = 0;
	int a[100] = {2, 3, 5, 7, 11, };
	if (N == 2)
		maxId = 0;
	else if (N < 5)
		maxId = 1;
	else if (N < 7)
		maxId = 2;
	else if (N < 11)
		maxId =3;
	else
	{
		maxId =3;
		int count = sizeof(a) / sizeof(a[0]);
		for (int i = 11; i <= N && maxId + 1 < count; i += 2)
		{
			int j = 3, flag = 0;
			while (j <= sqrt(i))
			{
				if (i % j == 0)
				{
					flag = 1;
					break;
				}
				++j;
			}
			if (flag == 0)
			{
				++maxId;
				a[maxId] = i;
			}
		}
	}

	printf("Result:\n");
	for (int i = 0; i <= maxId; ++i)
	{
		printf("%d\t", a[i]);
	}

	putchar('\n');
	return 0;
}

