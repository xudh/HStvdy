#include "Test3LayerB.h"
#include <stdio.h>

CTest3LayerB::CTest3LayerB()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

CTest3LayerB::CTest3LayerB(int serialNo)
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
        printf("serialNo = %d\n", serialNo);
}

CTest3LayerB::~CTest3LayerB()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3LayerB::VirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3LayerB::PureVirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3LayerB::VirtualFunc2()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3LayerB::PureVirtualFunc2()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3LayerB::DeepVirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3LayerB::DeepPureVirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3LayerB::ClassInnerFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
        CTest3Layer::CommonInnerFunc();
}

void CTest3LayerB::SomePriFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

