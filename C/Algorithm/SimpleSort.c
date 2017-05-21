#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

// 选择排序：每次从尚未排序的数组中选择最小的，放到前面已排序的数组中的最后位置
void SelectSort(int arr[], size_t count)
{
	for (size_t i = 0; i + 1 < count; ++i)
	{
		size_t minId = i;    // 假定最小的索引
		for (size_t j = i + 1; j < count; ++j)
		{
			if (arr[j] < arr[minId])
				minId = j;  	// 最终最小的索引
		}

		if (minId != i)		// 交换位置
		{
			int tmpVal = arr[i];
			arr[i] = arr[minId];
			arr[minId] = tmpVal;
		}
	}
}

// 插入排序：每次从尚未排序的数组中取首项，插入到前面已排序的数组中合适的位置
void InsertSort(int arr[], size_t count)
{
	for (size_t i = 1; i < count; ++i)	// 未排序的
	{
		int tmpVal = arr[i];
		int j = i - 1;
		for (; j >= 0; --j)	// 已排序的
		{
			if (arr[j] > tmpVal)
				arr[j + 1] = arr[j];	// 后移
			else
			{
				arr[j + 1] = tmpVal;
				break;
			}
		}

		if (j < 0)	// 插到最前
			arr[0] = tmpVal;
	}
}

/* 冒泡排序：每次通过比较并交换尚未排序的数组中的相邻元素，使最大元素冒到未排序的数组的最后，也就是已排序的数组首项的前一项
 * 冒泡排序兼具选择排序的“最大/小值”及插入排序的“相邻比较交换”的特点。*/

void BubbleSort(int arr[], size_t count)
{
	while (count > 0)
	{
		_Bool bNoExchange = true;	// 没有交换
		for (size_t j = 0; j + 1 < count; ++j)		// 每次在前count个数值中将最大的冒到最后(是count个数的最后，不是总的数的最后)
		{
			if (arr[j] > arr[j + 1])
			{
				int tmpVal = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmpVal;
				bNoExchange = false;
			}
		}

		if (bNoExchange)	// 没有交换，说明已经是有序的
			break;

		--count;
	}
}

int main(void)
{
	int testVal[3][20] = {{0, 1, 1, -1, -2, -3, 0, 9, 8, 6, 7, 11, -11, 22, -22, 44, -44, -44, 33, -5},
				{0, 1, 1, -1, -2, -3, 0, 9, 8, 6, 7, 11, -11, 22, -22, 44, -44, -44, 33, -5},
				{0, 1, 1, -1, -2, -3, 0, 9, 8, 6, 7, 11, -11, 22, -22, 44, -44, -44, 33, -5}};
	SelectSort(testVal[0], sizeof(testVal[0]) / sizeof(testVal[0][0]));
	for (size_t i = 0; i < sizeof(testVal[0]) / sizeof(testVal[0][0]); ++i)
	{
		printf("%d\t", testVal[0][i]);
	}
	printf("\n");

	InsertSort(testVal[1], sizeof(testVal[1]) / sizeof(testVal[1][0]));
	for (size_t i = 0; i < sizeof(testVal[1]) / sizeof(testVal[1][0]); ++i)
	{
		printf("%d\t", testVal[1][i]);
	}
	printf("\n");

	BubbleSort(testVal[2], sizeof(testVal[2]) / sizeof(testVal[2][0]));
	for (size_t i = 0; i < sizeof(testVal[2]) / sizeof(testVal[2][0]); ++i)
	{
		printf("%d\t", testVal[2][i]);
	}
	printf("\n");

	return 0;
}

