#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int t_iNum[18] = {0, };		// 身份证号码，对应的每位数字
const int t_iModeBasix = 11;	// 模运算的基数是11(? mod 11)
const int t_iFactor[17] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};	// 前17位数字对应的乘积因数
int t_iMonthDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};			// 平年某月的天数

bool GetOriginalNumbers(const char *trainTicketID)	// 获取原始数字，比如："0375"，分离成0, 3, 7, 5
{
	if(strlen(trainTicketID) == 18)
	{
		// 校验数字
		for(int i = 0; i < 18; ++i)	
		{
			if(i > 9 && i < 14)		// [10]->[13]位是星号
			{
				if(trainTicketID[i] != '*')
				{
					return false;
				}
			}
			else if(!isdigit(trainTicketID[i]) && (i != 17 || trainTicketID[i] != 'X'))	// 除了最后位可能为X, 其它位必须是数字
			{
				return false;	
			}
		}

		// 解析数字
		for(int i = 0; i < 18; ++i)	
		{
			if(i < 10 || i > 13)		// [10]->[13]位是星号
			{
				t_iNum[i] = (trainTicketID[i] == 'X') ? 10 : (trainTicketID[i] - '0');	// 'X'是罗马数字十
			}
		}
		
	}
	else
	{
		return false;
	}

	return true;
}

int GetYearNum()			// 获取年号
{
	return (t_iNum[6] * 1000 + t_iNum[7] * 100 + t_iNum[8] * 10 + t_iNum[9]);
}

bool IsBissextile(int year)		// 是否为闰年
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);	// 四年一闰，百年不闰，四百年再闰
}

int main(int argc, char *argv[])
{
	if(argc != 2 || argv[1] == '\0' || !GetOriginalNumbers(argv[1]))
	{
		printf("使用方式: %s 火车票上的身份证号码(如：1101011974****8839)\n", argv[0]);
		return -1;
	}

	char cNum[19] = "";
	strcpy(cNum, argv[1]);

	int iYear = GetYearNum();
	if(IsBissextile(iYear))
	{
		t_iMonthDay[1] += 1;
	}

	// 计算前17个数字中的已知数字的乘积和 
	int iSumKnow = 0;
	for(int i = 0; i < 17; ++i)	
	{
		if(i < 10 || i > 13)	// [10]->[13]位是星号
		{
			iSumKnow += t_iNum[i] * t_iFactor[i];
		}
	}

	// 遍历月份、日期寻找可能匹配的数字
	bool bFind = false;
	int iSumStar = 0;
	for(int i = 0; i < 12; ++i)
	{
		int iMonth = i + 1;
		for(int j = 1; j <= t_iMonthDay[i]; ++j)
		{
			iSumStar = (iMonth / 10) * t_iFactor[10] + (iMonth % 10) * t_iFactor[11] + (j / 10) * t_iFactor[12] + (j % 10) * t_iFactor[13];	
			if((iSumKnow + iSumStar + t_iNum[17]) % 11 == 1)	// 身份证核心算法
			{
				cNum[10] = iMonth / 10 + '0';
				cNum[11] = iMonth % 10 + '0';
				cNum[12] = j / 10 + '0';
				cNum[13] = j % 10 + '0';	
				printf("完整的身份证号码可能是：%s\t生日：%d月%d日\n", cNum, iMonth, j);
				bFind = true;
			}
		}
	}

	if(!bFind)
	{
		printf("无法推测出完整的身份证号码，请确定输入的数字无误！\n");
	}

	return 0;
}

