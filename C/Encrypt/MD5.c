#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct MD5Context
{ 
	uint32_t state[4]; 
	uint32_t count[2]; 
	uint8_t buffer[64]; 
};

#define F(x, y, z) (((x) & (y)) |((~x) & (z))) 
#define G(x, y, z) (((x) & (z)) |((y) & (~z))) 
#define H(x, y, z) ((x) ^ (y) ^ (z)) 
#define I(x, y, z) ((y) ^ ((x) | (~z))) 

#define ROTATE_LEFT(x, n)(((x) << (n)) |((x) >> (32 - (n)))) 

#define FF(a, b, c, d, x, s, ac) { (a) += F((b),(c),(d)) + (x) +(uint32_t)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b); } 
#define GG(a, b, c, d, x, s, ac) { (a) += G((b),(c),(d)) + (x) +(uint32_t)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b); } 
#define HH(a, b, c, d, x, s, ac) { (a) += H((b),(c),(d)) + (x) +(uint32_t)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b); } 
#define II(a, b, c, d, x, s, ac) { (a) += I((b),(c),(d)) + (x) +(uint32_t)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b); } 

static void MD5Init(struct MD5Context *pCtx) 
{ 
	memset(pCtx, 0, sizeof(*pCtx));
	pCtx->state[0] = 0x67452301; 
	pCtx->state[1] = 0xefcdab89; 
	pCtx->state[2] = 0x98badcfe; 
	pCtx->state[3] = 0x10325476; 
} 

static void MD5Transform(uint32_t (*pState)[4], const uint8_t (*pBlk)[64]) 
{ 
	uint32_t a = (*pState)[0], b = (*pState)[1], c = (*pState)[2], d = (*pState)[3];
	uint32_t x[16] = {0}; 
	memcpy(x, pBlk, sizeof(x));
	// 各16次
	FF(a, b, c, d, x[0], 7, 0xd76aa478);
	FF(d, a, b, c, x[1], 12, 0xe8c7b756);
	FF(c, d, a, b, x[2], 17, 0x242070db);
	FF(b, c, d, a, x[3], 22, 0xc1bdceee);
	FF(a, b, c, d, x[4], 7, 0xf57c0faf);
	FF(d, a, b, c, x[5], 12, 0x4787c62a);
	FF(c, d, a, b, x[6], 17, 0xa8304613);
	FF(b, c, d, a, x[7], 22, 0xfd469501);
	FF(a, b, c, d, x[8], 7, 0x698098d8);
	FF(d, a, b, c, x[9], 12, 0x8b44f7af);
	FF(c, d, a, b, x[10], 17, 0xffff5bb1);
	FF(b, c, d, a, x[11], 22, 0x895cd7be);
	FF(a, b, c, d, x[12], 7, 0x6b901122);
	FF(d, a, b, c, x[13], 12, 0xfd987193);
	FF(c, d, a, b, x[14], 17, 0xa679438e);
	FF(b, c, d, a, x[15], 22, 0x49b40821);
	GG(a, b, c, d, x[1], 5, 0xf61e2562);
	GG(d, a, b, c, x[6], 9, 0xc040b340);
	GG(c, d, a, b, x[11], 14, 0x265e5a51);
	GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);
	GG(a, b, c, d, x[5], 5, 0xd62f105d);
	GG(d, a, b, c, x[10], 9,  0x2441453);
	GG(c, d, a, b, x[15], 14, 0xd8a1e681);
	GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);
	GG(a, b, c, d, x[9], 5, 0x21e1cde6);
	GG(d, a, b, c, x[14], 9, 0xc33707d6);
	GG(c, d, a, b, x[3], 14, 0xf4d50d87);
	GG(b, c, d, a, x[8], 20, 0x455a14ed);
	GG(a, b, c, d, x[13], 5, 0xa9e3e905);
	GG(d, a, b, c, x[2], 9, 0xfcefa3f8);
	GG(c, d, a, b, x[7], 14, 0x676f02d9);
	GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);
	HH(a, b, c, d, x[5], 4, 0xfffa3942);
	HH(d, a, b, c, x[8], 11, 0x8771f681);
	HH(c, d, a, b, x[11], 16, 0x6d9d6122);
	HH(b, c, d, a, x[14], 23, 0xfde5380c);
	HH(a, b, c, d, x[1], 4, 0xa4beea44);
	HH(d, a, b, c, x[4], 11, 0x4bdecfa9);
	HH(c, d, a, b, x[7], 16, 0xf6bb4b60);
	HH(b, c, d, a, x[10], 23, 0xbebfbc70);
	HH(a, b, c, d, x[13], 4, 0x289b7ec6);
	HH(d, a, b, c, x[0], 11, 0xeaa127fa);
	HH(c, d, a, b, x[3], 16, 0xd4ef3085);
	HH(b, c, d, a, x[6], 23,  0x4881d05);
	HH(a, b, c, d, x[9], 4, 0xd9d4d039);
	HH(d, a, b, c, x[12], 11, 0xe6db99e5);
	HH(c, d, a, b, x[15], 16, 0x1fa27cf8);
	HH(b, c, d, a, x[2], 23, 0xc4ac5665);
	II(a, b, c, d, x[0], 6, 0xf4292244);
	II(d, a, b, c, x[7], 10, 0x432aff97);
	II(c, d, a, b, x[14], 15, 0xab9423a7);
	II(b, c, d, a, x[5], 21, 0xfc93a039);
	II(a, b, c, d, x[12], 6, 0x655b59c3);
	II(d, a, b, c, x[3], 10, 0x8f0ccc92);
	II(c, d, a, b, x[10], 15, 0xffeff47d);
	II(b, c, d, a, x[1], 21, 0x85845dd1);
	II(a, b, c, d, x[8], 6, 0x6fa87e4f);
	II(d, a, b, c, x[15], 10, 0xfe2ce6e0);
	II(c, d, a, b, x[6], 15, 0xa3014314);
	II(b, c, d, a, x[13], 21, 0x4e0811a1);
	II(a, b, c, d, x[4], 6, 0xf7537e82);
	II(d, a, b, c, x[11], 10, 0xbd3af235);
	II(c, d, a, b, x[2], 15, 0x2ad7d2bb);
	II(b, c, d, a, x[9], 21, 0xeb86d391);
	(*pState)[0] += a; 
	(*pState)[1] += b; 
	(*pState)[2] += c; 
	(*pState)[3] += d; 
} 

static void MD5Update(struct MD5Context *pCtx, const uint8_t *pInput, uint32_t inputLen) 
{ 
	uint32_t index = ((pCtx->count[0] >> 3) & 0x3F); 
	pCtx->count[0] += (inputLen << 3);
	if (pCtx->count[0] < (inputLen << 3)) 
		++pCtx->count[1]; 
	pCtx->count[1] += (inputLen >> 29); 
	uint32_t i = 0;
	uint32_t partLen = 64 - index; 
	if (inputLen >= partLen)
	{ 
		memcpy(&pCtx->buffer[index], pInput, partLen); 
		MD5Transform(&pCtx->state, (const uint8_t (*)[64])&pCtx->buffer); 
		for (i = partLen; i + 63 < inputLen; i += 64) 
			MD5Transform(&pCtx->state, (const uint8_t (*)[64])&pInput[i]); 
		index = 0; 
	} 

	memcpy(&pCtx->buffer[index], &pInput[i], inputLen - i); 
} 

static void MD5Final(struct MD5Context *pCtx) 
{ 
	uint8_t toByte[8] = {0};
	memcpy(toByte, pCtx->count, sizeof(toByte));
	uint32_t index = ((pCtx->count[0] >> 3) & 0x3f); 
	uint32_t padLen = ((index < 56) ? (56 - index) : (120 - index)); 
	static const uint8_t padding[68] = {0x80, };
	MD5Update(pCtx, padding, padLen); 
	MD5Update(pCtx, toByte, sizeof(toByte)); 
} 

void MD5String(char (*pDst)[33], const void *pSrc, size_t srcLen)
{
	memset(pDst, 0, sizeof(*pDst));
	struct MD5Context ctx;
	MD5Init(&ctx); 
	MD5Update(&ctx, (uint8_t *)pSrc, srcLen); 
	MD5Final(&ctx); 
	uint8_t *pDstInner = (uint8_t *)ctx.state;
	char *pC = (char *)pDst;
	for (int i = 0; i < 16; ++i) 
	{
		sprintf(pC, "%02x", pDstInner[i]);
		pC += 2;
	}
}

void MD5String16(char (*pDst)[17], const void *pSrc, size_t srcLen)
{
	memset(pDst, 0, sizeof(*pDst));
	struct MD5Context ctx;
	MD5Init(&ctx); 
	MD5Update(&ctx, (uint8_t *)pSrc, srcLen); 
	MD5Final(&ctx); 
	uint8_t *pDstInner = (uint8_t *)ctx.state;
	char *pC = (char *)pDst;
	for (int i = 4; i < 12; ++i)	// 取中间部分
	{
		sprintf(pC, "%02x", pDstInner[i]);
		pC += 2;
	}
}

int main(void)
{
	char testChr[][68] = {"a", "admin", "123456", "1234567", "12345678", "123456789", "汉字", "汉字&English",
							"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz12345678901",
							"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789012",
							"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890123"};
	char dst[33] = {0};
	for (size_t i = 0; i < sizeof(testChr) / sizeof(testChr[0]); ++i)
	{
		MD5String(&dst, testChr[i], strlen(testChr[i]));
		printf("MD5(%s) = %s\n", testChr[i], dst);
	}

	return 0;
}

