#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

const int t_iModeBasix = 11;		// 模运算的基数是11(? mod 11)
const int t_iFactor[3] = {8, 4, 2};	// 完整的系数是{7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2}, 这里是最后三个
int t_iONum[4] = {0, };			// 身份证号码最后四位
int t_iPreModeVal = 0;			// 前面14个数字的模数，

bool GetOriginalNumbers(const char *lastFourNumbers)	// 获取后四位单个数字，比如："0375"，分离成0, 3, 7, 5
{
	if(strlen(lastFourNumbers) == 4)
	{
		for(int i = 0; i < 4; ++i)	
		{
			if(isdigit(lastFourNumbers[i]))
			{
				t_iONum[i] = lastFourNumbers[i] - 0x30;	// '0'->0
			}
			else if(i == 3 && lastFourNumbers[i] == 'X')	// 最后一位可能是罗马数字'X'
			{
				t_iONum[i] = 10;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

void CalcPrevModeValue()		// 计算前14个数字的模数((前模数 + (15－17位*系数的和) + 18位)%模基==1)
{
	int iSum = 0;
	for(int i = 0; i < 3; ++i)
	{
		iSum += t_iONum[i] * t_iFactor[i];
	}

	iSum += t_iONum[3];
	iSum %= t_iModeBasix;
	t_iPreModeVal = (t_iModeBasix + 1 - iSum) % t_iModeBasix;
}

void ShiftMatch()				// 通过移动数字的位置，重新查找匹配方式
{
	int iSex = t_iONum[2] % 2;		// 性别
	bool bEndX = (t_iONum[3] == 10);	// 最后位是不是罗马数字'X'
	int iShiftNum = bEndX ? 3 : 4;		// 只有最后位能是'X'，所以为'X'时不能移动
	int iCount = 0;

	for(int i = 0; i < iShiftNum; ++i)	// 将四位数字排成t_iONum[i], [j], [k], [m]的形式
	{
		for(int j = 0; j < iShiftNum; ++j)
		{
			if(j == i)		// 不可重复使用某位
			{
				continue;
			}

			for(int k = 0; k < iShiftNum; ++k)
			{
				if(k == i || k == j || t_iONum[k] % 2 != iSex || (i == 0 && j == 1 && k == 2))	// 性别不能乱，且不可与原序列一模一样
				{
					continue;
				}

				for(int m = 0; m < 4; ++m)
				{
					if(m == i || m == j || m == k)
					{
						continue;
					}

					if(((t_iPreModeVal + t_iONum[i] * t_iFactor[0] + t_iONum[j] * t_iFactor[1] + t_iONum[k] * t_iFactor[2]) % t_iModeBasix + t_iONum[m])
						% t_iModeBasix == 1)	// 身份证核心校验算法
					{
						if(t_iONum[m] == 10)
						{
							printf("%d%d%dX\n", t_iONum[i], t_iONum[j], t_iONum[k]);
						}
						else
						{
							printf("%d%d%d%d\n", t_iONum[i], t_iONum[j], t_iONum[k], t_iONum[m]);
						}
						++iCount;
					}
				}
			}
		}
	}
	printf("通过移动数字，有%d种方式匹配。\n", iCount);
}

void ModifyTwoMatch()				// 通过修改两位数字，重新查找匹配方式(改一位校验肯定通不过的)
{
	int iSex = t_iONum[2] % 2;
	int iCount = 0;

	for(int i = 0; i < 4; ++i)		// 第一个修改位
	{
		int iBegin = (i == 2) ? iSex : 0;
		int iStep = (i == 2) ? 2 : 1;	// 性别是两位间隔
		int iEnd = (i == 3) ? 11 : 10;	// 最后位可以到'X'

		for(int iNum = iBegin;  iNum < iEnd;  iNum += iStep)	// 第一个修改位的变动范围
		{
			if(iNum == t_iONum[i])	// 不可不变
			{
				continue;
			}

			for(int j = i + 1; j < 4; ++j)			// 第二个修改位
			{
				int iBegin2 = (j == 2) ? iSex : 0;
				int iStep2 = (j == 2) ? 2 : 1;
				int iEnd2 = (j == 3) ? 11 : 10;

				for(int iNum2 = iBegin2;  iNum2 < iEnd2;  iNum2 += iStep2)	// 第二个修改位的变动范围
				{
					if(iNum2 == t_iONum[j] || (iNum2 == t_iONum[i] && iNum == t_iONum[j]))	// 不可不变，且不可互换
					{
						continue;
					}
					int iONumBak = t_iONum[i], iONumBak2 = t_iONum[j];
					t_iONum[i] = iNum, t_iONum[j] = iNum2;
					if(((t_iPreModeVal + t_iONum[0] * t_iFactor[0] + t_iONum[1] * t_iFactor[1] + t_iONum[2] * t_iFactor[2]) % t_iModeBasix + t_iONum[3])
						% t_iModeBasix == 1)
					{
						if(t_iONum[3] == 10)
						{
							printf("%d%d%dX\n", t_iONum[0], t_iONum[1], t_iONum[2]);
						}
						else
						{
							printf("%d%d%d%d\n", t_iONum[0], t_iONum[1], t_iONum[2], t_iONum[3]);
						}
						++iCount;
					}
					t_iONum[i] = iONumBak, t_iONum[j] = iONumBak2;
				}
			}
		}
	}
	printf("通过修改两个数字，有%d种方式匹配。\n", iCount);
}

int main(int argc, char *argv[])
{
	if(argc != 2 || argv[1] == '\0' || !GetOriginalNumbers(argv[1]))
	{
		printf("使用方式: %s 身份证号码后四位\n", argv[0]);
		return -1;
	}

	CalcPrevModeValue();
	ShiftMatch();
	printf("------------------------------\n");
	ModifyTwoMatch();
	printf("------------------------------\n");

	return 0;
}

