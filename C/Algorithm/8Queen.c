// 8皇后问题，不同行、不同列、不同斜线的棋子排放种类数
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#define QUEEN_NUM 8

static void PrintChess(int *pBoard, size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		for (size_t j = 0; j < count; ++j)
		{
			if (pBoard[i * count + j] != 0)
			{
				printf("%d\t", j + 1);
			}
		}
	}

	puts("");
}

// 能安全(不会互杀)放置
static _Bool SafeLay(int *pBoard, size_t count, size_t curRow, size_t column)
{
	// 回溯判断前面的值
	size_t rowDec = 0;	// 行差
	for (size_t i = 0; i < curRow; ++i)
	{
		if (pBoard[i * count + column] != 0)		// 正上方
			return false;
		rowDec = curRow - i;
		if (rowDec + column < count && pBoard[i * count + column + rowDec] != 0)	// 正斜向/
			return false;
		if (rowDec <= column && pBoard[i * count + column - rowDec] != 0)	// 反斜向，注意注释符后行尾的反斜杠\会解析
			return false;
	}

	return true;
}

// 放置棋子，返回总方案数，count是方阵的行数，curRow是当前行号
size_t LayChess(int *pBoard, size_t count, size_t curRow)
{
	if (count == 0)
		return 0;
	else if (count == 1)
	{
		pBoard[0] = 1;
		PrintChess(pBoard, 1);
		return 1;
	}

	size_t iRet = 0;
	if (curRow < 1)	// 首行
	{
		for (size_t i = 0; i < count; ++i)
		{
			pBoard[i] = 1;
			iRet += LayChess(pBoard, count, 1);
			pBoard[i] = 0;	// 复位，防止影响其它的
		}
	}
	else if (curRow + 1 >= count)	// 最后一行
	{
		for (size_t i = 0; i < count; ++i)
		{
			if (SafeLay(pBoard, count, count - 1, i))
			{
				pBoard[(count - 1) * count + i] = 1;
				PrintChess(pBoard, count);
				pBoard[(count - 1) * count + i] = 0;	// 复位，防止影响其它的
				++iRet;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < count; ++i)
		{
			if (SafeLay(pBoard, count, curRow, i))
			{
				pBoard[curRow * count + i] = 1;
				iRet += LayChess(pBoard, count, curRow + 1);
				pBoard[curRow * count + i] = 0;	// 复位，防止影响其它的
			}
		}
	}

	return iRet;
}

int main(void)
{
	int chessBoard[QUEEN_NUM][QUEEN_NUM] = {{0}};	// 行列数相等的方阵
	size_t ret = LayChess((int *)chessBoard, sizeof(chessBoard) / sizeof(chessBoard[0]), 0);
	printf("ret = %zu\n", ret);
	return 0;
}

