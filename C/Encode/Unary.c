// unary code: 一元编码，一进制编码，N=N个1后面加个0
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <inttypes.h>

#define MAX_BYTE 64

static void SetBit(uint8_t *pDst, size_t id)
{
	*pDst |= (1 << id);
}

void PrintByte(const uint8_t *pB, size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		printf("%02x ", pB[i]);
		if (i % 16 == 15)
			putchar('\n');
	}
	putchar('\n');
}

void PrintBit(const uint8_t *pB, size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		for (size_t j = 0; j < 8; ++j)
		{
			if (pB[i] & (1 << j))
				putchar('1');
			else
				putchar('0');
		}
		putchar(' ');
		if (i % 16 == 15)
			putchar('\n');
	}
	putchar('\n');
}

// 返回bit数，小于1表示失败，bitOff表示从第一字节的开始bit位存放结果
size_t UnaryEncodeOneByte(uint8_t *pDst, size_t dstSize, size_t bitOff, uint8_t src)
{
	if (bitOff > 7)
	{
		printf("%s:%d bitOff = %zu\n", __func__, __LINE__, bitOff);
		return 0;
	}

	size_t retBit = src + 1;
	if (retBit + bitOff > dstSize * 8)
	{
		printf("%s:%d dstSize = %zu, bitOff = %zu, src = %"PRIu8"\n", __func__, __LINE__, dstSize, bitOff, src);
		return 0;
	}

	uint8_t i = 0;
	uint8_t *pD = pDst;
	while (i < src && ((i + bitOff) % 8 != 0))
	{
		SetBit(pD, i + bitOff);
		++i;
	}

	if (i < src)
	{
		if (i > 0)	// 说明前面((i + bitOff) % 8 != 0填满字节了)
			++pD;
		uint8_t byte = (src - i) / 8;
		for (uint8_t j = 0; j < byte; ++j)
		{
			*pD = 0xff;
			++pD;
		}
		uint8_t leftBit = (src - i) % 8;
		for (uint8_t j = 0; j < leftBit; ++j)
		{
			SetBit(pD, j);
		}
	}

	// 最后bit清0
	uint8_t byte = (bitOff + retBit) / 8;
	uint8_t leftBit = (bitOff + retBit) % 8;
	*(pDst + byte) &= (0xff - (1<<leftBit)); 
	return retBit;
}

// 返回bit数，小于1表示失败，bitOff表示从第一字节的开始bit位存放结果
size_t UnaryEncode(uint8_t *pDst, size_t dstSize, size_t bitOff, uint8_t *pSrc, size_t srcLen)
{
	size_t retBit = 0, retBitTotal = 0;
	uint8_t byte = 0;
	for (size_t i = 0; i < srcLen; ++i)
	{
		retBit = UnaryEncodeOneByte(pDst, dstSize, bitOff, pSrc[i]);
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
size_t UnaryDecode(uint8_t *pDst, size_t dstSize, uint8_t *pSrc, size_t bitOff, size_t bitLen)
{
	if (bitLen < 1)
	{
		printf("%s:%d\n", __func__, __LINE__);
		return 0;
	}

	size_t srcByte = (bitLen + bitOff - 1) / 8;
	size_t srcBit = (bitLen + bitOff - 1) % 8;
	if (pSrc[srcByte] & (1 << srcBit))
	{
		printf("%s:%d last bit must be zero\n", __func__, __LINE__);
		return 0;
	}

	uint8_t val = 0;
	size_t i = 0, retByte = 0;
	for (i = 0; i < bitLen && retByte < dstSize; ++i)
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
			pDst[retByte] = val;
			++retByte;
			val = 0;
		}
	}

	if (i < bitLen)
	{
		printf("%s:%d dstSize = %zu isn't enough\n", __func__, __LINE__, dstSize);
		return 0;
	}

	return retByte;
}
int main(int argc, char *argv[])
{
	if (argc > 2)
	{
		if (argc > MAX_BYTE + 2)
		{
			printf("Argument's count can't more than %d\n",  MAX_BYTE + 1);
			return -1;
		}

		uint8_t src[MAX_BYTE] = {0}, dst[MAX_BYTE * 2] = {0};
		if (strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "-d") == 0)
		{
			for (size_t i = 0; i + 2 < argc; ++i)
			{
				src[i] = strtoul(argv[i + 2], NULL, 16);
			}
			if (strcmp(argv[1], "-e") == 0)
			{
				size_t retBit = UnaryEncode(dst, sizeof(dst), 0, src, argc - 2);
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
				uint8_t lastByte = src[argc - 3];
				if (lastByte & 0x80)
				{
					printf("%s:%d last bit must be zero\n", __func__, __LINE__);
					return 0;
				}
				int lastZero = 0;
				for (int i = 6; i >= 0; --i)
				{
					if (lastByte & (1<<i))
					{
						lastZero = i + 1;
						break;
					}
				}
				size_t bitLen = (argc - 3) * 8 + lastZero + 1;
				size_t retByte = UnaryDecode(dst, sizeof(dst), src, 0, bitLen);
				printf("retByte = %zu\n", retByte);
				if (retByte > 0)
					PrintByte(dst, retByte);
			}
			return 0;
		}
	}

	printf("Usage example: %s -e 03 0f 0a\n", argv[0]);
	return -1;
}

