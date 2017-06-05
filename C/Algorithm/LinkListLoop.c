// 判断链表是否有环路，并找出环路的入口
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NODE_NUM 100

struct Node
{
	int id;
	struct Node *pNext;
};

void TravelLinkList(struct Node *pNode)
{
	// 一个每次走两步，一个每次走一步，有环路会相遇
	struct Node *pJmp = pNode, *pStep = pNode;
	int jmpCnt = 0, stepCnt = 0;
	while (true)
	{
		if ((pJmp = pJmp->pNext) == NULL || (pJmp = pJmp->pNext) == NULL)	// 走两步
			break;

		pStep = pStep->pNext;
		if (pStep == pJmp)
		{
			printf("%s:%d meet, pStep->id = %d\n", __func__, __LINE__, pStep->id);
			// 原来每次走两步的改为从头开始每次走一步
			pJmp = pNode;
			while (true)
			{
				if (pStep == pJmp)	// 再次相遇，肯定在环的入口处
				{
					printf("%s:%d loop entrance = %d\n", __func__, __LINE__, pStep->id);
					return;
				}
				pJmp = pJmp->pNext;
				pStep = pStep->pNext;
			}
		}
	}

	printf("%s:%d no loop\n", __func__, __LINE__);
}

void TravelLinkListJudge(struct Node *pNode)	// 只要判断有环，用这个快些
{
	// 一个每次走两步，一个每次走一步，有环路会相遇
	struct Node *pJmp = pNode, *pStep = pNode;
	while (true)
	{
		if ((pJmp = pJmp->pNext) == NULL)
			break;
		else
		{
			if (pJmp != pStep)
			{
				if ((pJmp = pJmp->pNext) == NULL)	// 再走一步
					break;
			}
			if (pJmp == pStep)
			{
				printf("%s:%d meet, pJmp->id = %d\n", __func__, __LINE__, pJmp->id);
				return;
			}
		}

		pStep = pStep->pNext;
		if (pStep == pJmp)	// 慢的追上快的，肯定在环的入口处
		{
			printf("%s:%d loop entrance = %d\n", __func__, __LINE__, pStep->id);
			return;
		}
	}

	printf("%s:%d no loop\n", __func__, __LINE__);
}

int main(void)
{
	printf("LinkListLoop: Input loop Id(0-%d)\n", NODE_NUM);
	int loopId = 0;
	if (scanf("%d", &loopId) != 1 || loopId < 0 || loopId > NODE_NUM)
	{
		puts("Input error!");
		return -1;
	}

	struct Node *pNode = (struct Node *)calloc(NODE_NUM, sizeof(struct Node));
	for (int i = 0; i + 1 < NODE_NUM; ++i)
	{
		pNode[i].id = i;
		pNode[i].pNext = &pNode[i + 1];
	}

	pNode[NODE_NUM - 1].id = NODE_NUM - 1;
	if (loopId < NODE_NUM)
		pNode[NODE_NUM - 1].pNext = &pNode[loopId];

	TravelLinkList(pNode);
	return 0;
}

