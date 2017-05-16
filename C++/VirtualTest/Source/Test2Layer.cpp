#include "Test2Layer.h"
#include <stdio.h>

CTest2Layer::CTest2Layer()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

CTest2Layer::CTest2Layer(int serialNo)
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
        printf("serialNo = %d\n", serialNo);
}

CTest2Layer::~CTest2Layer()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

/*void CTest2Layer::Func()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}*/

/*void CTest2Layer::VirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}*/

void CTest2Layer::PureVirtualFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest2Layer::ClassInnerFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void CTest2Layer::SomePriFunc()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

ITest2Layer::ITest2Layer()                              //  必须实现，否则链接不过
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

ITest2Layer::ITest2Layer(int serialNo)  // 必须实现，否则链接不过
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
        printf("serialNo = %d\n", serialNo);
}

ITest2Layer::~ITest2Layer()                             //  必须实现(即使是纯虚)，否则链接不过
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void ITest2Layer::Func()                                //  必须实现，否则编译不过
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void ITest2Layer::VirtualFunc()                 //  必须实现，否则链接不过
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

/*void ITest2Layer::PureVirtualFunc()   // 不需要实现
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}*/

ITest2Layer *ITest2Layer::CreateObject(int serialNo)
{
        return (new CTest2Layer(serialNo));
}

