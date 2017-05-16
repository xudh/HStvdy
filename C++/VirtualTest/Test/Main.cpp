#include <stdio.h>
#include "VirtualTest/Test1Layer.h"
#include "VirtualTest/Test2Layer.h"
#include "VirtualTest/Test3Layer.h"

int main()
{
        printf("\n");
        ITest1Layer *pT1 = new ITest1Layer();
        pT1->Func();
        pT1->VirtualFunc();
        delete pT1;

        printf("\n\n");

        ITest2Layer *pT2 = ITest2Layer::CreateObject(101);	//  先后执行基类和派生类的构造函数
        pT2->Func();						//  执行的是基类的该函数
        pT2->VirtualFunc();					//  如果派生类覆盖了该函数执行的是派生类的
        pT2->PureVirtualFunc();					//  执行的是派生类的该函数
        delete pT2;						//  先后执行派生类和基类的析构函数

        printf("\n\n");

        // 以下: 由下层往上，如果派生类覆盖了该函数执行的是派生类的
        ITest3Layer *pT3 = ITest3Layer::CreateObject(100);
        pT3->VirtualFunc();
        pT3->PureVirtualFunc();
        pT3->VirtualFunc2();                    //  如果最下层派生类没实现，那么执行次下层派生类实现的
        pT3->PureVirtualFunc2();
        pT3->ShallowVirtualFunc();
        pT3->ShallowPureVirtualFunc();
        pT3->DeepVirtualFunc();
        pT3->DeepPureVirtualFunc();
        delete pT3;

        printf("\n");

        pT3 = ITest3Layer::CreateObject(101);
        pT3->VirtualFunc();
        pT3->PureVirtualFunc();
        pT3->VirtualFunc2();
        pT3->PureVirtualFunc2();
        pT3->ShallowVirtualFunc();
        pT3->ShallowPureVirtualFunc();
        pT3->DeepVirtualFunc();
        pT3->DeepPureVirtualFunc();
        delete pT3;

        printf("\n");

        return 0;
}

