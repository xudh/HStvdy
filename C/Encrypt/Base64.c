#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

int Base64Encode(char *pDst, size_t dstSize, const void *pSrc, size_t srcLen)
{
	if (((srcLen + 2) / 3) * 4 + 1 > dstSize)
	{
		printf("%s:%d srcLen = %zu, dstSize = %zu\n", __func__, __LINE__, srcLen, dstSize);
		return -1;
	}

	static const char base64Chr[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	const uint8_t *pS = (const uint8_t *)pSrc;
	for (; srcLen > 2; srcLen -= 3, pS += 3)
	{
		*pDst++ = base64Chr[pS[0] >> 2];
		*pDst++ = base64Chr[((pS[0] << 4) & 0x30) | (pS[1] >> 4)];
		*pDst++ = base64Chr[((pS[1] << 2) & 0x3c) | (pS[2] >> 6)];
		*pDst++ = base64Chr[pS[2] & 0x3f];
	}

	if (srcLen > 0)
	{
		*pDst++ = base64Chr[pS[0] >> 2];
		uint8_t fragment = (pS[0] << 4) & 0x30;
		if (srcLen > 1)
			fragment |= pS[1] >> 4;
		*pDst++ = base64Chr[fragment];
		*pDst++ = ((srcLen < 2) ? '=' : base64Chr[(pS[1] << 2) & 0x3c]);
		*pDst++ = '=';
	}

	*pDst = '\0';
	return 0;
}

long Base64Decode(void *pDst, size_t dstSize, const char *pSrc)	// 返回解码后的长度，大于0表示正确
{
	size_t len = strlen(pSrc);
	if (len % 4 != 0 || (len / 4) * 3 + 1 > dstSize)
	{
		printf("%s:%d len = %zu, dstSize = %zu\n", __func__, __LINE__, len, dstSize);
		return -1;
	}

	for (size_t i = 0; i < len; ++i)
	{
		if (pSrc[i] < 0)
		{
			printf("%s:%d pSrc[%zu] = %d\n", __func__, __LINE__, i, pSrc[i]);
			return -1;
		}
	}

	memset(pDst, 0, dstSize);
	static const int8_t base64Code[128] = 
	{	
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
		52, 53, 54, 55,	56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
		-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
		15, 16, 17, 18,	19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
		-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,  37, 38, 39, 40,
		41, 42, 43, 44,	45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
	};

	unsigned char *pS = (unsigned char *)pSrc;	// 要做数组下标用，防止编译警告
	uint8_t *pD = (uint8_t *)pDst;
	for (; len > 3; len -= 4, pS += 4)
	{
		if (base64Code[pS[0]] == -1 || base64Code[pS[1]] == -1
			|| (pS[2] != '=' && base64Code[pS[2]] == -1) || (pS[3] != '=' && base64Code[pS[3]] == -1))
		{
			printf("%s:%d pS[0-3] = %d%d%d%d\n", __func__, __LINE__, pS[0], pS[1], pS[2], pS[3]);
			return -1;
		}

		*pD++ = (base64Code[pS[0]] << 2) | (base64Code[pS[1]] >> 4);
		if (pS[2] == '=')
			break;
		else
		{
			*pD++ = ((base64Code[pS[1]] << 4) & 0xf0) | (base64Code[pS[2]] >> 2);
			if (pS[3] == '=')
				break;
			else
				*pD++ = ((base64Code[pS[2]] << 6) & 0xc0) | base64Code[pS[3]];
		}
	}

	if (len < 1)
		return (pD - (uint8_t *)pDst);
	else if (len != 4)
	{
		printf("%s:%d len = %zu\n", __func__, __LINE__, len);
		return -1;
	}
	else
	{
		if (pS[3] == '=')
			return (pD - (uint8_t *)pDst);
		else
		{
			printf("%s:%d pS[3] = %d\n", __func__, __LINE__, pS[3]);
			return -1;
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc == 3)
	{
		if (strcmp(argv[1], "-e") == 0)			// 加密
		{
			char dst[128] = {0};
			if (Base64Encode(dst, sizeof(dst), argv[2], strlen(argv[2])) == 0)
				printf("Base64Encode(%s) = %s\n", argv[2], dst);
			return 0;
		}
		else if (strcmp(argv[1], "-d") == 0)		// 解密
		{
			uint8_t dst2[128] = {0};
			long ret = Base64Decode(dst2, sizeof(dst2), argv[2]);
			if (ret > 0)
			{
				printf("Base64Decode(%s) = ", argv[2]);
				for (long i = 0; i < ret; ++i)
					printf("%02x ", dst2[i]);
				putchar('\n');
			}
			return 0;
		}
	}

	const char testChr[][64] = {"a", "ab", "abc", "abcd", "abcde", "abcdef", "abcdefg", "汉字", "汉字&English"};
	char dst[128] = {0};
	for (size_t i = 0; i < sizeof(testChr) / sizeof(testChr[0]); ++i)
	{
		if (Base64Encode(dst, sizeof(dst), testChr[i], strlen(testChr[i])) == 0)
		{
			printf("Base64Encode(%s) = %s\n", testChr[i], dst);
			uint8_t dst2[128] = {0};
			if (Base64Decode(dst2, sizeof(dst2), dst) > 0)
			{
				if (strcmp((const char *)dst2, testChr[i]) != 0)
					printf("Base64Decode(%s) = %s\n", dst, (const char *)dst2);
			}
		}
	}

	return 0;
}

