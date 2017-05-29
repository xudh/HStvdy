// 埃及分数，只使用分子为1的分数，正常非1的用几个分子为1的相加表示
#include <stdio.h>
#include <stdbool.h>

int main(void)
{
	int numerator = 1, denominator = 1, aDen = 1;
	puts("Input numerator and denominator");
	if (scanf("%d%d", &numerator, &denominator) != 2 || numerator < 1 || denominator < 1 || numerator >= denominator)
	{
		puts("Input error!");
		return -1;
	}

	printf("%d/%d = ", numerator, denominator);
	_Bool bFirst = true;
	while (true)
	{
		if (denominator % numerator == 0)
		{
			printf("%s1/%d\n", (bFirst ? "" : " + "), denominator / numerator);
			break;
		}

		aDen = denominator / numerator + 1;
		if (bFirst)
		{
			printf("1/%d", aDen);
			bFirst = false;
		}
		else
			printf(" + 1/%d", aDen);
		numerator = numerator * aDen - denominator;
		denominator *= aDen;
		if (numerator < 1 || denominator < 1 || numerator >= denominator)	// 整形溢出了
		{
			printf("\noverflow: numerator = %d, denominator = %d\n\n", numerator, denominator);
			break;
		}
	}
	
	return 0;
}

