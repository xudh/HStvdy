#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "TextConfig.h"

#define MAX_PATH 64
#define MAX_LINE_LEN 1024*3
#define MAX_SECTIONNAME_LEN 32
#define MAX_KEYNAME_LEN 32
#define MAX_VALUE_LEN 256

static const char split[3] = " \t";						// 分割字符集(用里面任一字节分割)
static const char notes[][3] = {";", "//", "--", "#"};	// 注释符号

struct FileInf
{
	char filePath[MAX_PATH];
	FILE *pFD;
	char lastSec[MAX_SECTIONNAME_LEN];
	long lastSecPos;
	pthread_mutex_t mutexRW;
};

static int tiCount = 0;					// 缓存文件指针个数
static struct FileInf tFiles[16] = {{{0}}};	// 缓存文件指针数组，最多支持这么多个
static _Bool tbFileClose = false;		// 关闭文件，比如在umount前
static pthread_mutex_t tMutexFiles = PTHREAD_MUTEX_INITIALIZER;

int32_t StrToInt(const char *pStr)
{
	int32_t ret = 0;
	if (strlen(pStr) > 2 && pStr[0] == '0' && (pStr[1] == 'x' || pStr[1] == 'X'))
		sscanf(pStr, "%"SCNx32, &ret);
	else
		sscanf(pStr, "%"SCNd32, &ret);

	return ret;
}

uint64_t StrToU64(const char *pStr)
{
	uint64_t ret = 0;
	if (strlen(pStr) > 2 && pStr[0] == '0' && (pStr[1] == 'x' || pStr[1] == 'X'))
		sscanf(pStr, "%"SCNx64, &ret);
	else
		sscanf(pStr, "%"SCNu64, &ret);

	return ret;
}

static void DistillLine(const char *pStrLine, size_t *start, size_t *len)	// 提取行字符串的有意义段
{
	*start = 0;
	*len = strlen(pStrLine);
	if (*len < 1)
		return;

	// 剔除字符串后面的换行符
	if (pStrLine[*len - 1] == '\n')
	{
		if (*len > 1)
		{
			if (pStrLine[*len - 2] == '\r')
				*len -= 2;
			else
				*len -= 1;
		}
		else
		{
			*len -= 1;
		}
	}
	else if (pStrLine[*len - 1] == '\r')
	{
		*len -= 1;
	}

	// 剔除字符串前面的无效字符(分割符)
	size_t i = 0;
	for (i = 0; i < *len; ++i)
	{
		if (strchr(split, pStrLine[i + *start]) == NULL)
			break;
	}

	*len -= i;
	*start += i;

	if (*len < 1)
		return;

	#if 0
	// 将注释符后面的字符清除掉
	char *pC = NULL;
	for (i = 0; i < sizeof(notes) / sizeof(notes[0]); ++i)
	{
		pC = strstr(pStrLine + *start, notes[i]);
		if (pC != NULL)
		{			
			if (pC == pStrLine + *start)
			{
				*len = 0;
				return;
			}
			else if (pC < pStrLine + *start + *len)
			{
				*len = pC - (pStrLine + *start);
			}
		}
	}
	#else
	// 注释符放在有效字符前面才有效，否则当做普通字符
	char *pC = NULL;
	for (i = 0; i < sizeof(notes) / sizeof(notes[0]); ++i)
	{
		pC = strstr(pStrLine + *start, notes[i]);
		if (pC == pStrLine + *start)
		{			
			*len = 0;
			return;
		}
	}
	#endif

	// 剔除字符串后面的无效字符(分割符)
	for (i = *len - 1; i > 0; --i)	// 首字符必定有效，所以i > 0
	{
		if (strchr(split, pStrLine[i + *start]) == NULL)
			break;
	}

	*len = i + 1;
}

static void GetArrayString(const char *pStr, size_t nIndex, size_t *start, size_t *len)
{
	*start = 0;
	*len = 0;

	size_t i = 0, j = 0;
	char *pC = NULL, *pCFront = NULL;
	for (i = 0; i < nIndex + 1; ++i)
	{
		// 找到第一个分割符作为字符串的结尾
		for (j = 0; j < strlen(split); ++j)
		{
			if ((pC = strchr(pStr + *start, split[j])) != NULL)
			{
				if (pCFront == NULL || pCFront > pC)
					pCFront = pC;
			}
		}
		if (pCFront == pStr + *start)
		{
			printf("%s: %s: %d shouldn't come here.\n", __FILE__, __func__, __LINE__);
			return;
		}
		else if (pCFront == NULL)	// 最后的字符串
		{
			if (i == nIndex)
				*len = strlen(pStr + *start);
			return;
		}
		else
		{
			if (i == nIndex)
			{
				*len = pCFront - (pStr + *start);
				return;
			}
			*start = pCFront + 1 - pStr;
			pCFront = NULL;
			// 跳过字符串前面的分割符
			while (strchr(split, *(pStr + *start)) != NULL)
				++(*start);
			if (*(pStr + *start) == '\0')
				return;
		}
	}
}

static long SeekToSection(FILE *pFD, const char *section)
{
	size_t start = 0, len = 0;
	char *pStr = NULL;
	char strLine[MAX_LINE_LEN] = "";
	rewind(pFD);
	long pos = 0;

	while (tbFileClose == false && (pStr = fgets(strLine, MAX_LINE_LEN, pFD)) != NULL)
	{
		// 段名是在中括号[]中的
		DistillLine(pStr, &start, &len);
		if (len > 2 && pStr[start] == '[' && pStr[start + len - 1] == ']')
		{
			if ((len - 2) == strlen(section) && strncasecmp(pStr + start + 1, section, len - 2) == 0)
				return pos + start;
		}
		pos = ftell(pFD);
		if (pos < 0)
			return -1;
	}

	return -1;
}

static long SeekToSectionPos(FILE *pFD, const char *section, long pos)
{
	if (fseek(pFD, pos, SEEK_SET) == 0)
	{	
		char *pStr = NULL;
		char strLine[MAX_LINE_LEN] = "";
		if ((pStr = fgets(strLine, MAX_LINE_LEN, pFD)) != NULL)
		{
			size_t len = strlen(section);
			if (strlen(pStr) >= len + 2 && pStr[0] == '[' && pStr[len + 1] == ']' && strncasecmp(pStr + 1, section, len) == 0)
				return pos;
		}
	}
	else
	{
		printf("%s: %s: %d errno = %d, means: %s\n", __FILE__, __func__, __LINE__, errno, strerror(errno));
	}

	return SeekToSection(pFD, section);
}

static long AddSection(FILE *pFD, const char *section)		// 返回段的位置
{
	// 调用前保证文件无错，确认指针移到文件尾
	long pos = ftell(pFD);
	if (pos < 0)
		return -1;

	if (fseek(pFD, 0, SEEK_END) != 0)
		return -1;
	long posFileEnd = ftell(pFD);	// 文件尾
	if (posFileEnd < 0 || posFileEnd != pos)
		return -1;

	char buf[MAX_SECTIONNAME_LEN + 4] = "";
	if (pos > 0)
		sprintf(buf, "\r\n[%s]\r\n", section);
	else
		sprintf(buf, "[%s]\r\n", section);
	if (fwrite(buf, strlen(buf), 1, pFD) == 1)
	{
		fflush(pFD);
		printf("%s: %s: %d create section %s success\n", __FILE__, __func__, __LINE__, section);
		if (pos > 0)
			return pos + 2;
		else
			return pos;
	}

	return -1;
}

static size_t GetItemNameLen(const char *pStrLine)		// 获取行字符串中item名的长度
{
	char *pC = strchr(pStrLine, '=');
	char *pSplit = NULL;
	size_t i = 0;
	for (i = 0; i < sizeof(split) / sizeof(split[0]); ++i)
	{
		pSplit = strchr(pStrLine, split[i]);
		if (pSplit != NULL)
		{
			if (pC == NULL || pSplit < pC)
				pC = pSplit;
		}
	}

	if (pC == NULL)
		return strlen(pStrLine);
	else
		return pC - pStrLine;
}

static int GetItemLine(FILE *pFD, const char *item, char *strLine, size_t maxLen, long *pos)	// pos记住行首位置
{
	size_t itemLen = strlen(item);
	size_t itemLenRead = 0;
	size_t start = 0, len = 0;
	char *pStr = NULL;
	*pos = ftell(pFD);
	if (*pos < 0)
		return -1;

	while (tbFileClose == false && (pStr = fgets(strLine, maxLen, pFD)) != NULL)
	{
		// 段名是在中括号[]中的
		DistillLine(pStr, &start, &len);
		if (len > 1 && pStr[start] == '[' && pStr[start + len - 1] == ']')		// 找到后面的段，说明本段找完了，还没找到
			return -1;

		if (len >= itemLen)
		{
			char bak = pStr[start + len];
			pStr[start + len] = '\0';
			itemLenRead = GetItemNameLen(pStr + start);
			if (itemLenRead == itemLen && strncasecmp(pStr + start, item, itemLen) == 0)
			{
				pStr[start + len] = bak;
				return 0; 
			}
		}

		*pos = ftell(pFD);
		if (*pos < 0)
			return -1;
	}

	return -1;
}

static void DistillItemValue(const char *pStrLine, size_t *start, size_t *len)	// 提取行字符串中的item值段
{
	char *pC = strchr(pStrLine, '=');
	if (pC == NULL)
	{
		*len = 0;
		return;
	}

	*start = pC + 1 - pStrLine;
	*len = strlen(pC + 1);

	// 剔除字符串前面的无效字符(分割符)
	size_t i = 0;
	for (i = 0; i < *len; ++i)
	{
		if (strchr(split, pStrLine[i + *start]) == NULL)
			break;
	}

	*len -= i;
	*start += i;
}

static void GetItemValue(const char *pStrLine, char *strValue, size_t maxLen)
{
	size_t start = 0, len = 0;
	DistillItemValue(pStrLine, &start, &len);
	if (len > 0)
	{
		if (len > maxLen - 1)
			memcpy(strValue, pStrLine + start, maxLen - 1);
		else
			memcpy(strValue, pStrLine + start, len);
	}
}

static int FileTailLineFeed(FILE *pFD)			// 文件尾部必须要有换行符
{
	int ret = -1;
	if (fseek(pFD, 0, SEEK_END) == 0)
	{
		long posFileEnd = ftell(pFD);	// 文件尾
		if (posFileEnd == 0)
			ret = 0;
		else if (posFileEnd > 0)
		{
			if (fseek(pFD, -1, SEEK_CUR) == 0)
			{
				char buf[2] = "";
				if (fread(buf, 1, 1, pFD) == 1)
				{
					if (buf[0] == '\n')
						ret = 0;
					else if (fwrite("\r\n", 2, 1, pFD) == 1)
					{
						fflush(pFD);
						ret = 0;
						printf("%s:%s:%d add CR+LF after tail.\n", __FILE__, __func__, __LINE__);
					}
				}
			}
		}
	}

	return ret;
}

static int OpenFile(const char *filePath)		// 返回文件号
{
	int fileId = -1;
	pthread_mutex_lock(&tMutexFiles);
	if (tbFileClose == false)
	{
		size_t i = 0;
		for (i = 0; i < tiCount; ++i)
		{
			if (strcmp(filePath, tFiles[i].filePath) == 0)
				break;
		}

		if (i < sizeof(tFiles) / sizeof(tFiles[0]))
		{
			fileId = i;
			if (i >= tiCount)		// 新创建
			{
				strcpy(tFiles[i].filePath, filePath);
				pthread_mutex_init(&tFiles[i].mutexRW, NULL);
				++tiCount;
			}
		}
			
	}
	pthread_mutex_unlock(&tMutexFiles);

	if (fileId > -1 && tbFileClose == false)
	{
		pthread_mutex_lock(&tFiles[fileId].mutexRW);
		if (tFiles[fileId].pFD == NULL && tbFileClose == false)
		{
			printf("%s: %s: %d filePath = %s\n", __FILE__, __func__, __LINE__, filePath);
			_Bool bReadOnly = false;
			tFiles[fileId].pFD = fopen(filePath, "rb+");
			if (tFiles[fileId].pFD == NULL)
			{
				if (errno == EROFS && tbFileClose == false)	// 只读
				{
					tFiles[fileId].pFD = fopen(filePath, "rb");
					if (tFiles[fileId].pFD == NULL)
						printf("%s: %s: %d errno = %d, means: %s\n", __FILE__, __func__, __LINE__, errno, strerror(errno));
					else
						bReadOnly = true;
				}
				else if (errno == ENOENT && tbFileClose == false)	// 文件不存在时创建
				{
					tFiles[fileId].pFD = fopen(filePath, "wb+");
					if (tFiles[fileId].pFD == NULL)
						printf("%s: %s: %d errno = %d, means: %s\n", __FILE__, __func__, __LINE__, errno, strerror(errno));
				}
				else
					printf("%s: %s: %d errno = %d, means: %s\n", __FILE__, __func__, __LINE__, errno, strerror(errno));
			}
			if (tFiles[fileId].pFD != NULL && bReadOnly == false)
			{
				// 确认结尾换行，否则后面每次写入都要判断
				if (FileTailLineFeed(tFiles[fileId].pFD) != 0)
				{
					fclose(tFiles[fileId].pFD);
					tFiles[fileId].pFD = NULL;
				}
			}
		}
		pthread_mutex_unlock(&tFiles[fileId].mutexRW);
	}

	return fileId;
}

static int ModifyFile(FILE *pFD, const char *pStr, size_t preLen)	// preLen表示文件中要被覆盖的长度
{
	long posLine = ftell(pFD);		// 行首
	if (posLine < 0)
		return -1;

	size_t len = strlen(pStr);
	if (len == preLen)
	{
		if (len == 0)
			return 0;
		else if (fwrite(pStr, len, 1, pFD) == 1)
		{
			fflush(pFD);
			return 0;
		}
		else
		{
			printf("%s: %s: %d errno = %d, means: %s\n", __FILE__, __func__, __LINE__, errno, strerror(errno));
			return -1;
		}
	}

	long posLineEnd = posLine;		// 行尾
	if (preLen > 0)
	{
		if (fseek(pFD, preLen, SEEK_CUR) != 0)
			return -1;
		posLineEnd = ftell(pFD);
		if (posLineEnd < 0 || posLineEnd < posLine)
			return -1;
	}

	if (fseek(pFD, 0, SEEK_END) != 0)
		return -1;
	long posFileEnd = ftell(pFD);	// 文件尾
	if (posFileEnd < 0 || posFileEnd < posLineEnd)
			return -1;

	if (tbFileClose)
		return -1;

	if (posFileEnd <= posLineEnd)
	{
		if (fseek(pFD, posLine, SEEK_SET) != 0)
			return -1;
		if (len > 1 && fwrite(pStr, len, 1, pFD) != 1)
		{
			printf("%s: %s: %d errno = %d, means: %s\n", __FILE__, __func__, __LINE__, errno, strerror(errno));
			return -1;
		}

		fflush(pFD);
	}
	else
	{
		if (fseek(pFD, posLineEnd, SEEK_SET) != 0)
			return -1;
		char *pBuf = (char *)malloc(posFileEnd - posLineEnd);
		if (pBuf == NULL)
		{
			printf("%s: %s: %d errno = %d, means: %s\n", __FILE__, __func__, __LINE__, errno, strerror(errno));
			return -1;
		}

		// 先把要修改的行后面的内容读出来缓存
		if (fread(pBuf, posFileEnd - posLineEnd, 1, pFD) != 1)
		{
			printf("%s: %s: %d errno = %d, means: %s\n", __FILE__, __func__, __LINE__, errno, strerror(errno));
			free(pBuf);
			return -1;
		}
		if (tbFileClose)
		{
			free(pBuf);
			return -1;
		}
		if (fseek(pFD, posLine, SEEK_SET) != 0)
		{
			free(pBuf);
			return -1;
		}
		// 写入新数据
		if (len > 0 && fwrite(pStr, len, 1, pFD) != 1)
		{
			printf("%s: %s: %d errno = %d, means: %s\n", __FILE__, __func__, __LINE__, errno, strerror(errno));
			free(pBuf);
			return -1;
		}
		// 缓存写回去
		if (fwrite(pBuf, posFileEnd - posLineEnd, 1, pFD) != 1)
		{
			printf("%s: %s: %d errno = %d, means: %s\n", __FILE__, __func__, __LINE__, errno, strerror(errno));
			free(pBuf);
			return -1;
		}
		fflush(pFD);
		free(pBuf);
	}

	if (len < preLen)	// 文件变小
	{
		long posFileEnd = ftell(pFD);	// 文件尾
		if (posFileEnd < 0)
				return -1;
	
		fflush(pFD);
		rewind(pFD);
		int fd = fileno(pFD);
		ftruncate(fd, posFileEnd);
	}

	return 0;
}

int TxtCfgGetString(const char *filePath, const char *section, const char *item, char *strOutVal, size_t nMaxStrLen)
{
	if (tbFileClose)
		return -1;

	if (filePath == NULL || section == NULL || item == NULL
		|| strlen(filePath) < 1 || strlen(section) < 1 || strlen(item) < 1
		|| strlen(filePath) > MAX_PATH - 1 || strlen(section) > MAX_SECTIONNAME_LEN - 1 || strlen(item) > MAX_KEYNAME_LEN - 1
		|| strOutVal == NULL || nMaxStrLen < 2)
	{
		printf("%s: Invalid param\n", __func__);
		return -1;
	}

	memset(strOutVal, 0, nMaxStrLen);

	int fileId = OpenFile(filePath);
	if (fileId < 0)
		return -1;

	int ret = -1;
	pthread_mutex_lock(&tFiles[fileId].mutexRW);
	if (tbFileClose == false && tFiles[fileId].pFD != NULL)
	{
		long pos = 0;
		if (strcmp(tFiles[fileId].lastSec, section) == 0)
			pos = SeekToSectionPos(tFiles[fileId].pFD, section, tFiles[fileId].lastSecPos);
		else
			pos = SeekToSection(tFiles[fileId].pFD, section);

		if (pos < 0)
		{
			memset(tFiles[fileId].lastSec, 0, sizeof tFiles[fileId].lastSec);
			pthread_mutex_unlock(&tFiles[fileId].mutexRW);
			return -1;
		}

		strcpy(tFiles[fileId].lastSec, section);
		tFiles[fileId].lastSecPos = pos;

		char strLine[MAX_LINE_LEN] = "";
		if (GetItemLine(tFiles[fileId].pFD, item, strLine, sizeof strLine, &pos) == 0)
		{
			size_t start = 0, len = 0;
			DistillLine(strLine, &start, &len);
			strLine[start + len] = '\0';
			GetItemValue(strLine, strOutVal, nMaxStrLen);
			ret = 0;
		}
	}
	pthread_mutex_unlock(&tFiles[fileId].mutexRW);

	return ret;
}

int TxtCfgGetValue(const char *filePath, const char *section, const char *item, int32_t *nOutVal)
{
	if (nOutVal == NULL)
	{
		printf("%s: Invalid param\n", __func__);
		return -1;
	}

	char strValue[16] = "";
	if (TxtCfgGetString(filePath, section, item, strValue, sizeof strValue) == 0)
	{
		*nOutVal = StrToInt(strValue);
		return 0;
	}

	return -1;
}

int TxtCfgGetValueU64(const char *filePath, const char *section, const char *item, uint64_t *nOutVal)
{
	if (nOutVal == NULL)
	{
		printf("%s: Invalid param\n", __func__);
		return -1;
	}

	char strValue[32] = "";
	if (TxtCfgGetString(filePath, section, item, strValue, sizeof strValue) == 0)
	{
		*nOutVal = StrToU64(strValue);
		return 0;
	}

	return -1;
}

int TxtCfgGetArrayStrings(const char *filePath, const char *section, const char *item, char *strOutVal[], size_t *nItems, size_t nTableLen, size_t nMaxStrLen)
{
	if (strOutVal == NULL || nItems == NULL || nTableLen < 1 || nMaxStrLen < 2)
	{
		printf("%s: Invalid param\n", __func__);
		return -1;
	}

	char strValue[MAX_VALUE_LEN] = "";
	if (TxtCfgGetString(filePath, section, item, strValue, sizeof strValue) == 0)
	{
		size_t i = 0, j = 0;
		char *pC = NULL, *pCFront = NULL;
		char *pStr = strValue;
		for (i = 0; i < nTableLen; ++i)
		{
			memset(strOutVal[i], 0, nMaxStrLen);
			// 找到第一个分割符作为字符串的结尾
			for (j = 0; j < strlen(split); ++j)
			{
				if ((pC = strchr(pStr, split[j])) != NULL)
				{
					if (pCFront == NULL || pCFront > pC)
						pCFront = pC;
				}
			}
			if (pCFront == pStr)
			{
				printf("%s: %d shouldn't come here.\n", __func__, __LINE__);
				return -1;
			}
			else if (pCFront == NULL)	// 最后的字符串
			{
				strncpy(strOutVal[i], pStr, nMaxStrLen - 1);
				++i;
				break;
			}
			else
			{
				*pCFront = '\0';
				strncpy(strOutVal[i], pStr, nMaxStrLen - 1);
				pStr = pCFront + 1;
				pCFront = NULL;
				// 跳过字符串前面的分割符
				while (strchr(split, *pStr) != NULL)
				{
					++pStr;
				}
				if (*pStr == '\0')
				{
					++i;
					break;
				}
			}
		}

		*nItems = i;
		return 0;
	}

	return -1;
}

int TxtCfgGetArrayValues(const char *filePath, const char *section, const char *item, int32_t nOutVal[], size_t *nItems, size_t nTableLen)
{
	if (nOutVal == NULL || nItems == NULL || nTableLen < 1)
	{
		printf("%s: Invalid param\n", __func__);
		return -1;
	}

	memset(nOutVal, 0, sizeof(nOutVal[0]) * nTableLen);
	char strValues[MAX_VALUE_LEN/2][16] = {{0}};
	char *pStrV[MAX_VALUE_LEN/2] = {NULL};
	int i = 0;
	for (i = 0; i < MAX_VALUE_LEN/2; ++i)
		pStrV[i] = strValues[i];
	size_t strCount = 0;
	if (TxtCfgGetArrayStrings(filePath, section, item, pStrV, &strCount, MAX_VALUE_LEN/2, 16) == 0)
	{
		if (strCount > nTableLen)
			strCount = nTableLen;
		int i = 0;
		for (i = 0; i < strCount; ++i)
		{
			nOutVal[i] = StrToInt(pStrV[i]);
		}
		*nItems = strCount;
		return 0;
	}

	return -1;
}

int TxtCfgGetArrayValuesU64(const char *filePath, const char *section, const char *item, uint64_t nOutVal[], size_t *nItems, size_t nTableLen)
{
	if (nOutVal == NULL || nItems == NULL || nTableLen < 1)
	{
		printf("%s: Invalid param\n", __func__);
		return -1;
	}

	memset(nOutVal, 0, sizeof(nOutVal[0]) * nTableLen);
	char strValues[MAX_VALUE_LEN/2][32] = {{0}};
	char *pStrV[MAX_VALUE_LEN/2] = {NULL};
	int i = 0;
	for (i = 0; i < MAX_VALUE_LEN/2; ++i)
		pStrV[i] = strValues[i];
	size_t strCount = 0;
	if (TxtCfgGetArrayStrings(filePath, section, item, pStrV, &strCount, MAX_VALUE_LEN/2, 32) == 0)
	{
		if (strCount > nTableLen)
			strCount = nTableLen;
		int i = 0;
		for (i = 0; i < strCount; ++i)
		{
			nOutVal[i] = StrToU64(pStrV[i]);
		}
		*nItems = strCount;
		return 0;
	}

	return -1;
}

int TxtCfgGetArrayString(const char *filePath, const char *section, const char *item, uint16_t nIndex, char *strOutVal, size_t nMaxStrLen)
{
	if (strOutVal == NULL || nMaxStrLen < 2)
	{
		printf("%s: Invalid param\n", __func__);
		return -1;
	}

	memset(strOutVal, 0, nMaxStrLen);

	char strValue[MAX_VALUE_LEN] = "";
	if (TxtCfgGetString(filePath, section, item, strValue, sizeof strValue) == 0)
	{
		size_t start = 0, len = 0;
		GetArrayString(strValue, nIndex, &start, &len);
		if (len  < 1)
			return -1;

		if (len > nMaxStrLen - 1)
			len = nMaxStrLen - 1;
		strncpy(strOutVal, strValue + start, len);
		return 0;
	}

	return -1;
}

int TxtCfgGetArrayValue(const char *filePath, const char *section, const char *item, uint16_t nIndex, int32_t *nOutVal)
{
	if (nOutVal == NULL)
	{
		printf("%s: Invalid param\n", __func__);
		return -1;
	}

	char strValue[16] = "";
	if (TxtCfgGetArrayString(filePath, section, item, nIndex, strValue, sizeof strValue) == 0)
	{
		*nOutVal = StrToInt(strValue);
		return 0;
	}

	return -1;
}

int TxtCfgGetArrayValueU64(const char *filePath, const char *section, const char *item, uint16_t nIndex, uint64_t *nOutVal)
{
	if (nOutVal == NULL)
	{
		printf("%s: Invalid param\n", __func__);
		return -1;
	}

	char strValue[32] = "";
	if (TxtCfgGetArrayString(filePath, section, item, nIndex, strValue, sizeof strValue) == 0)
	{
		*nOutVal = StrToU64(strValue);
		return 0;
	}

	return -1;
}

int TxtCfgSetString(const char *filePath, const char *section, const char *item, const char *strNewVal)
{
	if (tbFileClose)
		return -1;

	if (filePath == NULL || section == NULL || item == NULL
		|| strlen(filePath) < 1 || strlen(section) < 1 || strlen(item) < 1
		|| strlen(filePath) > MAX_PATH - 1 || strlen(section) > MAX_SECTIONNAME_LEN - 1 || strlen(item) > MAX_KEYNAME_LEN - 1
		|| strNewVal == NULL || strlen(strNewVal) > MAX_VALUE_LEN - 1)
	{
		printf("%s: Invalid param\n", __func__);
		return -1;
	}

	int fileId = OpenFile(filePath);
	if (fileId < 0)
		return -1;

	int ret = -1;
	pthread_mutex_lock(&tFiles[fileId].mutexRW);
	if (tbFileClose == false && tFiles[fileId].pFD != NULL)
	{
		long pos = 0;
		if (strcmp(tFiles[fileId].lastSec, section) == 0)
			pos = SeekToSectionPos(tFiles[fileId].pFD, section, tFiles[fileId].lastSecPos);
		else
			pos = SeekToSection(tFiles[fileId].pFD, section);

		if (pos < 0 && tbFileClose == false)
			pos = AddSection(tFiles[fileId].pFD, section);

		if (pos < 0)
		{
			memset(tFiles[fileId].lastSec, 0, sizeof tFiles[fileId].lastSec);
			pthread_mutex_unlock(&tFiles[fileId].mutexRW);
			return -1;
		}

		strcpy(tFiles[fileId].lastSec, section);
		tFiles[fileId].lastSecPos = pos;

		char strLine[MAX_LINE_LEN] = "";
		if (GetItemLine(tFiles[fileId].pFD, item, strLine, sizeof strLine, &pos) == 0)
		{
			size_t lineLen = strlen(strLine);	// 记住原来的行长度
			size_t start = 0, len = 0, start2 = 0;
			DistillLine(strLine, &start, &len);
			strLine[start+len] = '\0';
			DistillItemValue(strLine + start, &start2, &len);
			if (len != strlen(strNewVal) || strncmp(strLine + start + start2, strNewVal, len) != 0)
			{
				if (fseek(tFiles[fileId].pFD, pos, SEEK_SET) == 0)	// 行首位置
				{
					
					sprintf(strLine, "%s=%s\r\n", item, strNewVal);
					ret = ModifyFile(tFiles[fileId].pFD, strLine, lineLen);
				}
				else
					printf("%s: %s: %d errno = %d, means: %s\n", __FILE__, __func__, __LINE__, errno, strerror(errno));
			}
			else
				ret = 0;
		}
		else
		{
			if (tbFileClose == false)
			{
				sprintf(strLine, "%s=%s\r\n", item, strNewVal);
				// 新增的项放在文件尾或者跟在段后面(已有的项前面)
				if (feof(tFiles[fileId].pFD) != 0)
				{
					ret = (fputs(strLine, tFiles[fileId].pFD) > -1) ? 0 : -1;
					fflush(tFiles[fileId].pFD);
				}
				else
				{
					pos = SeekToSectionPos(tFiles[fileId].pFD, section, tFiles[fileId].lastSecPos);
					if (pos < 0)
					{
						memset(tFiles[fileId].lastSec, 0, sizeof tFiles[fileId].lastSec);
						pthread_mutex_unlock(&tFiles[fileId].mutexRW);
						return -1;
					}
					ret = ModifyFile(tFiles[fileId].pFD, strLine, 0);
				}
			}
		}
	}
	pthread_mutex_unlock(&tFiles[fileId].mutexRW);

	return ret;
}

int TxtCfgSetValue(const char *filePath, const char *section, const char *item, int32_t nNewVal)
{
	char strValue[16] = "";
	sprintf(strValue, "%"PRId32, nNewVal);
	return TxtCfgSetString(filePath, section, item, strValue);
}

int TxtCfgSetValueU64(const char *filePath, const char *section, const char *item, uint64_t nNewVal)
{
	char strValue[32] = "";
	sprintf(strValue, "%"PRId64, nNewVal);
	return TxtCfgSetString(filePath, section, item, strValue);
}

int TxtCfgSetArrayString(const char *filePath, const char *section, const char *item, uint16_t nIndex, const char *strNewVal)
{
	if (tbFileClose)
		return -1;

	if (filePath == NULL || section == NULL || item == NULL
		|| strlen(filePath) < 1 || strlen(section) < 1 || strlen(item) < 1
		|| strlen(filePath) > MAX_PATH - 1 || strlen(section) > MAX_SECTIONNAME_LEN - 1 || strlen(item) > MAX_KEYNAME_LEN - 1
		|| strNewVal == NULL || strlen(strNewVal) > MAX_VALUE_LEN - 1)
	{
		printf("%s: Invalid param\n", __func__);
		return -1;
	}

	int fileId = OpenFile(filePath);
	if (fileId < 0)
		return -1;

	int ret = -1;
	pthread_mutex_lock(&tFiles[fileId].mutexRW);
	if (tbFileClose == false && tFiles[fileId].pFD != NULL)
	{
		long pos = 0;
		if (strcmp(tFiles[fileId].lastSec, section) == 0)
			pos = SeekToSectionPos(tFiles[fileId].pFD, section, tFiles[fileId].lastSecPos);
		else
			pos = SeekToSection(tFiles[fileId].pFD, section);

		if (pos < 0 && tbFileClose == false)
			pos = AddSection(tFiles[fileId].pFD, section);

		if (pos < 0)
		{
			memset(tFiles[fileId].lastSec, 0, sizeof tFiles[fileId].lastSec);
			pthread_mutex_unlock(&tFiles[fileId].mutexRW);
			return -1;
		}

		strcpy(tFiles[fileId].lastSec, section);
		tFiles[fileId].lastSecPos = pos;

		char strLine[MAX_LINE_LEN] = "";
		if (GetItemLine(tFiles[fileId].pFD, item, strLine, sizeof strLine, &pos) == 0)
		{
			size_t lineLen = strlen(strLine);	// 记住原来的行长度
			size_t start = 0, len = 0, start2 = 0, len2 = 0, start3 = 0, len3 = 0;
			DistillLine(strLine, &start, &len);
			strLine[start+len] = '\0';
			DistillItemValue(strLine + start, &start2, &len2);
			strLine[start+start2+len2] = '\0';
			GetArrayString(strLine + start + start2, nIndex, &start3, &len3);
			_Bool modify = false;
			size_t modify_start = 0, modify_len = 0;
			if (len3  < 1)
			{
				if (len2 > 0)
				{
					if (nIndex < 1)
					{
						printf("%s: %s: %d shouldn't come here.\n", __FILE__, __func__, __LINE__);
						pthread_mutex_unlock(&tFiles[fileId].mutexRW);
						return -1;
					}

					size_t i = 0;
					for (i = nIndex - 1; i > 0; --i)
					{
						GetArrayString(strLine + start + start2, i, &start3, &len3);	// 找到最后一个有值的项
						if (len3 > 0)
							break;
					}
					if (i < 1)
					{
						start3 = 0;
						len3 = len2;	// 只存在一项
					}
					memset(strLine, 0, sizeof strLine);
					size_t j = 0;
					for (j = 0; j < nIndex - 1 - i; ++j)	// 前面没有的项用0填
					{
						strcat(strLine, " 0");
					}
					strcat(strLine, " ");
					strcat(strLine, strNewVal);
					modify_start = start + start2 + start3 + len3;
				}
				else
				{
					strcpy(strLine, item);
					strcat(strLine, "=");
					size_t i = 0;
					for (i = 0; i < nIndex; ++i)		// 前面没有的项用0填
					{
						strcat(strLine, "0 ");
					}
					strcat(strLine, strNewVal);
					strcat(strLine, "\r\n");
					modify_len = lineLen;
				}
				modify = true;
			}
			else
			{
				if (len3 != strlen(strNewVal) || strncmp(strLine + start + start2 + start3, strNewVal, len3) != 0)
				{
					modify_start = start + start2 + start3;
					sprintf(strLine, "%s", strNewVal);
					modify_len = len3;
					modify = true;
				}
			}

			if (modify)
			{
				if (fseek(tFiles[fileId].pFD, pos + modify_start, SEEK_SET) == 0)
					ret = ModifyFile(tFiles[fileId].pFD, strLine, modify_len);
				else
				{
					printf("%s: %s: %d errno = %d, means: %s\n", __FILE__, __func__, __LINE__, errno, strerror(errno));
					ret = -1;
				}
			}
			else
				ret = 0;
		}
		else
		{
			if (tbFileClose == false)
			{
				strcpy(strLine, item);
				strcat(strLine, "=");
				size_t i = 0;
				for (i = 0; i < nIndex; ++i)		// 前面没有的项用0填
				{
					strcat(strLine, "0 ");
				}
				strcat(strLine, strNewVal);
				strcat(strLine, "\r\n");

				// 新增的项放在文件尾或者跟在段后面(已有的项前面)
				if (feof(tFiles[fileId].pFD) != 0)
				{
					ret = (fputs(strLine, tFiles[fileId].pFD) > -1) ? 0 : -1;
					fflush(tFiles[fileId].pFD);
				}
				else
				{
					pos = SeekToSectionPos(tFiles[fileId].pFD, section, tFiles[fileId].lastSecPos);
					if (pos < 0)
					{
						memset(tFiles[fileId].lastSec, 0, sizeof tFiles[fileId].lastSec);
						pthread_mutex_unlock(&tFiles[fileId].mutexRW);
						return -1;
					}

					ret = ModifyFile(tFiles[fileId].pFD, strLine, 0);
				}
			}
		}
	}
	pthread_mutex_unlock(&tFiles[fileId].mutexRW);

	return ret;
}

int TxtCfgSetArrayValue(const char *filePath, const char *section, const char *item, uint16_t nIndex, int32_t nNewVal)
{
	char strValue[16] = "";
	sprintf(strValue, "%"PRId32, nNewVal);
	return TxtCfgSetArrayString(filePath, section, item, nIndex, strValue);
}

int TxtCfgSetArrayValueU64(const char *filePath, const char *section, const char *item, uint16_t nIndex, uint64_t nNewVal)
{
	char strValue[32] = "";
	sprintf(strValue, "%"PRId64, nNewVal);
	return TxtCfgSetArrayString(filePath, section, item, nIndex, strValue);
}

void TxtCfgClose(void)
{
	printf("%s:%s enter\n", __FILE__, __func__);
	pthread_mutex_lock(&tMutexFiles);
	tbFileClose = true;
	int i = 0;
	int fd = 0;
	for (i = 0; i < tiCount; ++i)
	{
		pthread_mutex_lock(&tFiles[i].mutexRW);
		if (tFiles[i].pFD != NULL)
		{
			fd = fileno(tFiles[i].pFD);
			fsync(fd);
			fclose(tFiles[i].pFD);
			tFiles[i].pFD = NULL;
		}
		pthread_mutex_unlock(&tFiles[i].mutexRW);
	}
	pthread_mutex_unlock(&tMutexFiles);
	printf("%s:%s exit\n", __FILE__, __func__);
}

void TxtCfgReOpen(void)
{
	printf("%s:%s enter\n", __FILE__, __func__);
	pthread_mutex_lock(&tMutexFiles);
	tbFileClose = false;
	pthread_mutex_unlock(&tMutexFiles);
	printf("%s:%s exit\n", __FILE__, __func__);
}

void TxtCfgCloseFile(const char *filePath)
{
	printf("%s:%s enter filePath = %s\n", __FILE__, __func__, filePath);
	if (tbFileClose)
		return;
	if (filePath == NULL || strlen(filePath) < 1 || strlen(filePath) > MAX_PATH - 1)
	{
		printf("%s: Invalid param\n", __func__);
		return;
	}

	int fileId = -1;
	pthread_mutex_lock(&tMutexFiles);
	if (tbFileClose == false)
	{
		size_t i = 0;
		for (i = 0; i < tiCount; ++i)
		{
			if (strcmp(filePath, tFiles[i].filePath) == 0)
				break;
		}
		if (i < tiCount && i < sizeof(tFiles)/sizeof(tFiles[0]))
			fileId = i;
	}
	pthread_mutex_unlock(&tMutexFiles);

	if (fileId != -1 && tbFileClose == false)
	{
		pthread_mutex_lock(&tFiles[fileId].mutexRW);
		if (tFiles[fileId].pFD != NULL)
		{
			int fd = fileno(tFiles[fileId].pFD);
			fsync(fd);
			fclose(tFiles[fileId].pFD);
			tFiles[fileId].pFD = NULL;
		}
		pthread_mutex_unlock(&tFiles[fileId].mutexRW);
	}
	printf("%s:%s exit\n", __FILE__, __func__);
}

