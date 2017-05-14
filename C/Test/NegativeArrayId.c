// 数组下标为负数的处理
#include <stdio.h>

int main(void)
{
	int iArr[3] = {2017, -5, -29};
	printf("&iArr[0] = %p, &iArr[1] = %p, &iArr[2] = %p, &iArr[-1] = %p, &iArr[-2] = %p\n", &iArr[0], &iArr[1], &iArr[2], &iArr[-1], &iArr[-2]);
	return 0;
}

// 结论：
// 1、C/C++里面数组下标是int类型的。
// 2、如果是负数则相当于指针向数组开始位置即&iArr[0]前面移动，寻找对应的值。

