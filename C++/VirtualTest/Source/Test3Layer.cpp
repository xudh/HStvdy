#include "Test3Layer.h"
#include <stdio.h>
#include "Test3LayerA.h"
#include "Test3LayerB.h"

CTest3Layer::CTest3Layer()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

CTest3Layer::~CTest3Layer()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3Layer::VirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3Layer::PureVirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3Layer::PureVirtualFunc2()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3Layer::ShallowVirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3Layer::ShallowPureVirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest3Layer::OtherVirtualFunc()
{
        printf("%s, %d, %s\n", __FILE__, __LINE__, __func__);
}

void CTest3Layer::CommonInnerFunc()
{
        printf("%s, %d, %s\n", __FILE__, __LINE__, __func__);
}

void CTest3Layer::SomePriFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

ITest3Layer::ITest3Layer()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

ITest3Layer::~ITest3Layer()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void ITest3Layer::VirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void ITest3Layer::VirtualFunc2()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void ITest3Layer::ShallowVirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void ITest3Layer::DeepVirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

ITest3Layer *ITest3Layer::CreateObject(int serialNo)
{
        if (serialNo % 2 == 0)
        {
                return (new CTest3LayerA(serialNo));
        }
        else
        {
                return (new CTest3LayerB(serialNo));
        }
}

