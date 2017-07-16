// 哥伦布编码，及其两个变种：Golomb-Rice和Exp-Golomb
// http://www.cnblogs.com/wangguchangqing/p/6297792.html
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include "Unary.h"

#define MAX_BYTE 64

static void SetBit(uint8_t *pDst, size_t id, _Bool oneOrZero)
{
	if (oneOrZero)
		*pDst |= (1 << id);
	else
		*pDst &= (0xff - (1 << id));
}

static uint8_t IntLog2(uint8_t num)	// 整形的2为底的对数，外部保证num是2的乘方
{
	for (uint8_t i = 0; i < 8; ++i)
	{
		if (num & (1 << i))
			return i;
	}

	printf("%s:%d shouldn't come here\n", __func__, __LINE__);
	return 0;
}

// 返回bit数，小于1表示失败，bitOff表示从第一字节的开始bit位存放结果
size_t GolombRiceEncodeOneByte(uint8_t *pDst, size_t dstSize, size_t bitOff, uint8_t src, uint8_t m)
{
	if (bitOff > 7)
	{
		printf("%s:%d bitOff = %zu\n", __func__, __LINE__, bitOff);
		return 0;
	}

	if (m < 2 || (m & (m - 1)) != 0)	// 判断m有校，且是2的乘方
	{
		printf("%s:%d m = %"PRIu8"\n", __func__, __LINE__, m);
		return 0;
	}

	if (m > (1 << 6))	// 这种情况下数据会变长，没有压缩效果
	{
		printf("%s:%d m = %"PRIu8"\n", __func__, __LINE__, m);
		return 0;
	}

	uint8_t groupId = src / m;
	size_t retBit = UnaryEncodeOneByte(pDst, dstSize, bitOff, groupId);
	if (retBit < 1)
		return 0;
	uint8_t byte = (bitOff + retBit) / 8;
	dstSize -= byte;
	pDst += byte;
	bitOff = (bitOff + retBit) % 8;
	uint8_t tailBit = IntLog2(m);
	uint8_t i = 0;
	while (i < tailBit && ((i + bitOff) % 8 != 0))
	{
		SetBit(pDst, i + bitOff, (1 << (tailBit - 1 - i)) & src);
		++i;
	}

	if (i < tailBit)
	{
		if (i > 0)	// 说明前面((i + bitOff) % 8 != 0填满字节了)
		{
			if (dstSize < 1)
			{
				printf("%s:%d\n", __func__, __LINE__);
				return 0;
			}
			--dstSize;
			++pDst;
		}
		uint8_t leftBit = tailBit - i;
		for (uint8_t j = 0; j < leftBit; ++j)
		{
			SetBit(pDst, j, (1 << (tailBit - 1 - j - i)) & src);	// 要逆序
		}
	}

	return (retBit + tailBit);
}

// 返回bit数，小于1表示失败，bitOff表示从第一字节的开始bit位存放结果
size_t GolombRiceEncode(uint8_t *pDst, size_t dstSize, size_t bitOff, uint8_t *pSrc, size_t srcLen, uint8_t m)
{
	size_t retBit = 0, retBitTotal = 0;
	uint8_t byte = 0;
	for (size_t i = 0; i < srcLen; ++i)
	{
		retBit = GolombRiceEncodeOneByte(pDst, dstSize, bitOff, pSrc[i], m);
		if (retBit < 1)
			return 0;
		byte = (bitOff + retBit) / 8;
		dstSize -= byte;
		pDst += byte;
		bitOff = (bitOff + retBit) % 8;
		retBitTotal += retBit;
	}

	return retBitTotal;
}

// 返回Byte数，小于1表示失败，bitOff表示从第一字节的开始bit位存放源数据
size_t GolombRiceDecode(uint8_t *pDst, size_t dstSize, uint8_t *pSrc, size_t bitOff, size_t bitLen, uint8_t m)
{
	if (bitLen < 1)
	{
		printf("%s:%d\n", __func__, __LINE__);
		return 0;
	}

	if (m < 2 || (m & (m - 1)) != 0)	// 判断m有校，且是2的乘方
	{
		printf("%s:%d m = %"PRIu8"\n", __func__, __LINE__, m);
		return 0;
	}

	uint8_t val = 0;
	size_t i = 0, retByte = 0, srcByte = 0, srcBit = 0;
	uint8_t tailBit = IntLog2(m);
	for (i = 0; i + tailBit < bitLen && retByte < dstSize; ++i)
	{
		srcByte = (i + bitOff) / 8;
		srcBit = (i + bitOff) % 8;
		if (pSrc[srcByte] & (1 << srcBit))
		{
			if (val == 0xff)
			{
				printf("%s:%d overflow\n", __func__, __LINE__);
				return 0;
			}
			++val;
		}
		else
		{
			pDst[retByte] = (val << tailBit);
			++i;
			uint8_t j = 0;
			while (j < tailBit)
			{
				srcByte = (i + bitOff + tailBit - 1 - j) / 8;	// 要逆序
				srcBit = (i + bitOff + tailBit - 1 - j) % 8;
				SetBit(&pDst[retByte], j, pSrc[srcByte] & (1 << srcBit));
				++j;
			}
			i += tailBit - 1;
			++retByte;
			val = 0;
		}
	}

	if (i < bitLen)
	{
		if (i + tailBit < bitLen)	// dstSize不够
		{
			printf("%s:%d dstSize = %zu, retByte = %zu, i = %zu, bitLen = %zu\n", __func__, __LINE__, dstSize, retByte, i, bitLen);
			return 0;
		}
		else
		{
			srcByte = (i + bitOff) / 8;
			srcBit = (i + bitOff) % 8;
			if (pSrc[srcByte] & (1 << srcBit)) // 数据不完整
			{
				printf("%s:%d dstSize = %zu, retByte = %zu, i = %zu, bitLen = %zu\n", __func__, __LINE__, dstSize, retByte, i, bitLen);
				return 0;
			}
		}
	}

	return retByte;
}

int main(int argc, char *argv[])
{
	if (argc > 5 && strcmp(argv[1], "rice") == 0 && (strcmp(argv[2], "-e") == 0 || strcmp(argv[2], "-d") == 0)
		&& strcmp(argv[3], "-m") == 0)
	{
		if (argc > MAX_BYTE + 5)
		{
			printf("Argument's count can't more than %d\n",  MAX_BYTE + 4);
			return -1;
		}

		int m = atoi(argv[4]);
		if (m < 0 || m > 0xff)
		{
			printf("m = %d, must great than 0 and little than 255\n", m);
			return -1;
		}

		uint8_t src[MAX_BYTE] = {0}, dst[MAX_BYTE * 2] = {0};
		for (size_t i = 0; i + 5 < argc; ++i)
		{
			src[i] = strtoul(argv[i + 5], NULL, 16);
		}
		if (strcmp(argv[2], "-e") == 0)
		{
			size_t retBit = GolombRiceEncode(dst, sizeof(dst), 0, src, argc - 5, (uint8_t)m);
			printf("retBit = %zu\n", retBit);
			if (retBit > 0)
			{
				size_t useByte = (retBit + 7) / 8;
				PrintByte(dst, useByte);
				PrintBit(dst, useByte);
			}
		}
		else
		{
			size_t bitLen = (argc - 5) * 8;
			size_t retByte = GolombRiceDecode(dst, sizeof(dst), src, 0, bitLen, (uint8_t)m);
			printf("retByte = %zu\n", retByte);
			if (retByte > 0)
				PrintByte(dst, retByte);
		}

		return 0;
	}

	printf("Usage example: %s -e -m 4 03 0f 0a\n", argv[0]);
	return -1;
}

