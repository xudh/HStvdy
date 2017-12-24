#ifndef TEXT_CONFIG___H
#define TEXT_CONFIG___H

/* 以文本的格式保存配置的一些读写函数，文件格式如下
 * [section1]
 * item1 = value
 * item2 = value1 value2
 * */

#include <stddef.h>
#include <stdint.h>

/* 以下函数返回0表示成功
 * filePath, section, item, strNewVal含结尾符长度限制分别为64, 32, 32, 256
 * strNewVal中不能出现' ', '\t', ':', "//", "--", '#'，否则读取时会解析异常
 * */

int TxtCfgGetString(const char *filePath, const char *section, const char *item, char *strOutVal, size_t nMaxStrLen);
int TxtCfgGetValue(const char *filePath, const char *section, const char *item, int32_t *nOutVal);
int TxtCfgGetValueU64(const char *filePath, const char *section, const char *item, uint64_t *nOutVal);
int TxtCfgGetArrayStrings(const char *filePath, const char *section, const char *item, char *strOutVal[], size_t *nItems, size_t nTableLen, size_t nMaxStrLen);
int TxtCfgGetArrayValues(const char *filePath, const char *section, const char *item, int32_t nOutVal[], size_t *nItems, size_t nTableLen);
int TxtCfgGetArrayValuesU64(const char *filePath, const char *section, const char *item, uint64_t nOutVal[], size_t *nItems, size_t nTableLen);
int TxtCfgGetArrayString(const char *filePath, const char *section, const char *item, uint16_t nIndex, char *strOutVal, size_t nMaxStrLen);
int TxtCfgGetArrayValue(const char *filePath, const char *section, const char *item, uint16_t nIndex, int32_t *nOutVal);
int TxtCfgGetArrayValueU64(const char *filePath, const char *section, const char *item, uint16_t nIndex, uint64_t *nOutVal);
int TxtCfgSetString(const char *filePath, const char *section, const char *item, const char *strNewVal);
int TxtCfgSetValue(const char *filePath, const char *section, const char *item, int32_t nNewVal);
int TxtCfgSetValueU64(const char *filePath, const char *section, const char *item, uint64_t nNewVal);
int TxtCfgSetArrayString(const char *filePath, const char *section, const char *item, uint16_t nIndex, const char *strNewVal);
int TxtCfgSetArrayValue(const char *filePath, const char *section, const char *item, uint16_t nIndex, int32_t nNewVal);
int TxtCfgSetArrayValueU64(const char *filePath, const char *section, const char *item, uint16_t nIndex, uint64_t nNewVal);

void TxtCfgClose(void);	// 程序将要关闭时调用，将修改刷到存储介质中
void TxtCfgReOpen(void);	// 程序将关未关时调用
void TxtCfgCloseFile(const char *filePath);	// 关闭指定文件
#endif

