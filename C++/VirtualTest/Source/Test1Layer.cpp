#include "VirtualTest/Test1Layer.h"
#include <stdio.h>

ITest1Layer::ITest1Layer()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

ITest1Layer::~ITest1Layer()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void ITest1Layer::Func()
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

void ITest1Layer::VirtualFunc() // 虚函数也可以自己实现
{
        printf("%s, %d, %s, %p\n", __FILE__, __LINE__, __func__, this);
}

