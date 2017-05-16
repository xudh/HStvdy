#ifndef __INTERFACE_TEST_1LAYER_H__
#define __INTERFACE_TEST_1LAYER_H__

class ITest1Layer
{
public:
        ITest1Layer();                                  // public 外面才能构造
        //virtual ITest1Layer();                 // 构造函数不能声明为虚函数
        //virtual ITest1Layer() = 0;

        ~ITest1Layer();                                 //  必须public，否则外面无法释放
        //virtual ~ITest1Layer();                // 前面加virtual，效果一样
        //virtual ~ITest1Layer() =& nbsp;0;   // 拥有存虚函数，不能创建对象

public:
        void Func();
        virtual void VirtualFunc();
        //virtual void PureVirtualFunc() =& nbsp;0;   // 拥有存虚函数，不能创建对象
};

#endif

