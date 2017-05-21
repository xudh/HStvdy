#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static int CmpFunc(const void *p1, const void *p2)
{
	if (*(int *)p1 == *(int *)p2)
		return 0;
	else
		return ((*(int *)p1 > *(int *)p2) ? 1 : -1);
}

static void QuickSort(int arr[], size_t count)		// 简单实现
{
	if (count < 2)
		return;
	size_t i = 0, j = count - 1;
	int key = arr[i];
	while (i < j)
	{
		while (j > i)
		{
			if (arr[j] < key)
			{
				arr[i] = arr[j];
				break;
			}
			--j;
		}
		while (i < j)
		{
			if (arr[i] > arr[j])
			{
				arr[j] = arr[i];
				break;
			}
			++i;
		}
	}
	arr[i] = key;
	if (i > 1)
		QuickSort(arr, i);
	if (i + 2 < count)
		QuickSort(arr + i + 1,  count - 1 - i);
}

int main(void)
{
	int testVal[2][20] = {{0, 1, 1, -1, -2, -3, 0, 9, 8, 6, 7, 11, -11, 22, -22, 44, -44, -44, 33, -5},
				{0, 1, 1, -1, -2, -3, 0, 9, 8, 6, 7, 11, -11, 22, -22, 44, -44, -44, 33, -5}};
	qsort(testVal[0], sizeof(testVal[0]) / sizeof(testVal[0][0]), sizeof(testVal[0][0]), CmpFunc);
	for (size_t i = 0; i < sizeof(testVal[0]) / sizeof(testVal[0][0]); ++i)
	{
		printf("%d\t", testVal[0][i]);
	}
	printf("\n");

	QuickSort(testVal[1], sizeof(testVal[1]) / sizeof(testVal[1][0]));
	for (size_t i = 0; i < sizeof(testVal[1]) / sizeof(testVal[1][0]); ++i)
	{
		printf("%d\t", testVal[1][i]);
	}
	printf("\n");
	return 0;
}

