#include <stdio.h>
#include <string.h>

#define MAX_SIZE 512

static void GetNextVal(const char *patternStr, int nextVal[])
{
	int id = 0, preJmp = -1;
	nextVal[0] = -1;
	while (id < strlen(patternStr))
	{
		if (preJmp == -1 || patternStr[id] == patternStr[preJmp])
		{
			++id, ++preJmp;
			if (patternStr[id] == patternStr[preJmp])
				nextVal[id] = preJmp;
			else
				nextVal[id] = nextVal[preJmp];
		}
		else
			preJmp = nextVal[preJmp];
	}
}

int KMPIndex(const char *mainStr, const char *patternStr)
{
	int mId = 0, pId = 0;
	int mLen = strlen(mainStr);
	int pLen = strlen(patternStr);
	int nextVal[MAX_SIZE] = {0};
	GetNextVal(patternStr, nextVal);
	while (mId <= mLen - pLen && pId < pLen)
	{
		if (pId == -1 || patternStr[pId] == mainStr[mId])
			++mId, ++pId;
		else
			pId = nextVal[pId];
	}

	return ((pId >= pLen) ? (mId - pLen) : -1);
}

int main(void)
{
	const char *mStr = "1236ABCDABCDabcababEFGHIJKLMNOPQRST";
	const char *pStr = "abab";
	int ret = KMPIndex(mStr, pStr);
	printf("ret = %d\n", ret);
	return 0;
}

