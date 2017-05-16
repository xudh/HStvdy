#ifndef __TEST_2LAYER_H__
#define __TEST_2LAYER_H__

#include "VirtualTest/Test2Layer.h"

class CTest2Layer : public ITest2Layer
{
private:
        CTest2Layer();
public:
        CTest2Layer(int serialNo);
//private:                                               // 若仅供libVirtualTest.a外部使用，private也行
        ~CTest2Layer();

        //void Func();                           // 无法覆盖
        //void VirtualFunc();            // 可选覆盖
        void PureVirtualFunc();         //  必须覆盖，否则因存在纯虚函数，不能创建对象

public:
        void ClassInnerFunc();

private:
        void SomePriFunc();
};

#endif

