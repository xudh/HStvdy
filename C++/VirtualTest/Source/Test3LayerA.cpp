#include "Test3LayerA.h"
#include <stdio.h>

CTest3LayerA::CTest3LayerA()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

CTest3LayerA::CTest3LayerA(int serialNo)
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
        printf("serialNo = %d\n", serialNo);
}

CTest3LayerA::~CTest3LayerA()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3LayerA::VirtualFunc2()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3LayerA::DeepPureVirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3LayerA::ClassInnerFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
        CTest3Layer::CommonInnerFunc();
}

void CTest3LayerA::SomePriFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

