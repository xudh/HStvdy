#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>
#include "TextConfig.h"

static const char *tpFile = "XX.cfg";

int main(void)
{
	if (TxtCfgSetString(tpFile, "Sect1", "item0", "Hello") != 0)
		printf("%s:%d\n", __func__, __LINE__);
	if (TxtCfgSetValue(tpFile, "Sect1", "item1", 10086) != 0)
		printf("%s:%d\n", __func__, __LINE__);
	if (TxtCfgSetValueU64(tpFile, "Sect1", "item2", 100861234567890178ull) != 0)
		printf("%s:%d\n", __func__, __LINE__);
	if (TxtCfgSetArrayString(tpFile, "Sect2", "item3", 0, "ABC") != 0)
		printf("%s:%d\n", __func__, __LINE__);
	if (TxtCfgSetArrayString(tpFile, "Sect2", "item3", 1, "Space^Space") != 0)
		printf("%s:%d\n", __func__, __LINE__);
	if (TxtCfgSetArrayValue(tpFile, "Sect2", "item3", 2, -121) != 0)
		printf("%s:%d\n", __func__, __LINE__);

	char strVal[16] = "";
	if (TxtCfgGetString(tpFile, "Sect1", "item0", strVal, sizeof(strVal)) != 0)
		printf("%s:%d\n", __func__, __LINE__);
	else
		printf("%s:%d strVal = %s.\n", __func__, __LINE__, strVal);
	int32_t iVal = 0;
	if (TxtCfgGetValue(tpFile, "Sect1", "item1", &iVal) != 0)
		printf("%s:%d\n", __func__, __LINE__);
	else
		printf("%s:%d iVal = %"PRId32"\n", __func__, __LINE__, iVal);
	uint64_t iVal64 = 0;
	if (TxtCfgGetValueU64(tpFile, "Sect1", "item2", &iVal64) != 0)
		printf("%s:%d\n", __func__, __LINE__);
	else
		printf("%s:%d iVal = %"PRIu64"\n", __func__, __LINE__, iVal64);
	memset(strVal, 0, sizeof(strVal));
	if (TxtCfgGetArrayString(tpFile, "Sect2", "item3", 0, strVal, sizeof(strVal)) != 0)
		printf("%s:%d\n", __func__, __LINE__);
	else
		printf("%s:%d strVal = %s.\n", __func__, __LINE__, strVal);
	if (TxtCfgGetArrayString(tpFile, "Sect2", "item3", 1, strVal, sizeof(strVal)) != 0)
		printf("%s:%d\n", __func__, __LINE__);
	else
		printf("%s:%d strVal = %s.\n", __func__, __LINE__, strVal);
	if (TxtCfgGetArrayValue(tpFile, "Sect2", "item3", 2, &iVal) != 0)
		printf("%s:%d\n", __func__, __LINE__);
	else
		printf("%s:%d iVal = %"PRId32"\n", __func__, __LINE__, iVal);

	TxtCfgClose();

	return 0;
}

