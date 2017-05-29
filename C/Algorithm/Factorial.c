// 阶乘，计算N!
#include <stdio.h>

#define MAX_N 1000

void Factorial(int N)
{
	if (N < 2)
		printf("%d! = 1\n", N);
	else
	{
		int fac[MAX_N] = {1};	// 从低到高，每个值存储4位数字
		int facUse = 1;			// 使用了多少个fac值
		for (int i = 1; i <= N; ++i)
		{
			int carryVal = 0;		// 进位值
			int facMulti = 0;		// fac和当前被求阶数乘
			for (int j = 0; j < facUse; ++j)
			{
				facMulti = carryVal + fac[j] * i;
				fac[j] = facMulti % 10000;
				carryVal = facMulti / 10000;
			}

			if (carryVal > 0)
			{
				fac[facUse] = carryVal;
				++facUse;
			}
		}
		printf("%d! = ", N);
		printf("%d", fac[facUse - 1]);
		for (int i = facUse - 2; i >= 0; --i)
		{
			printf("%04d", fac[i]);
		}
		putchar('\n');
	}
}

int main(void)
{
	int N = 0;
	printf("Factorial: Input factor N(<= %d)\n", MAX_N);
	if (scanf("%d", &N) != 1 || N < 0 || N > MAX_N)
	{
		puts("Input error!");
		return -1;
	}

	Factorial(N);
	return 0;
}

