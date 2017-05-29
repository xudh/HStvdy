// 约瑟夫环，N个人围成一圈，从1开始报数到M的退出，然后继续从1到M报数
#include <stdio.h>
#include <string.h>

#define MAX_N 1000

void Josephus(int N, int M)
{
	int num[MAX_N] = {0};
	for (int i = 0; i < N; ++i)
		num[i] = i + 1;

	puts("The exit queue is:");
	int exitId = 0;
	for (int i = N; i > 0; --i)
	{
		exitId = (exitId + M - 1) % i;
		printf("%d\t", num[exitId]);
		if (exitId + 1 < i)
			memmove(&num[exitId], &num[exitId + 1], sizeof(num[0]) * (i - exitId - 1));
	}
	putchar('\n');
}

void JosephusWinner(int N, int M)	// 只输出最后的赢者
{
	int w = 0;
	for (int i = 2; i <= N; ++i)
		w = (w + M) % i;
	printf("The winner is %d\n", w + 1);
}
int main(void)
{
	printf("Josephus: Input total number(<= %d) and exit number\n", MAX_N);
	int N = 1, M = 1;
	if (scanf("%d%d", &N, &M) != 2 || M < 1 || N < 1 || N > MAX_N)
	{
		puts("Input error!");
		return -1;
	}

	Josephus(N, M);
	JosephusWinner(N, M);
	return 0;
}

