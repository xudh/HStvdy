#ifndef __INTERFACE_TEST_2LAYER_H__
#define __INTERFACE_TEST_2LAYER_H__

class ITest2Layer
{
protected:
        ITest2Layer();                                  // protected, public 里面才能继承

private:
        ITest2Layer(int serialNo);

public:
        virtual ~ITest2Layer();                 //  必须public，否则外面无法释放
        //virtual ~ITest2Layer() = 0;   // 纯虚函数效果一样

        void Func();
        virtual void VirtualFunc();
        virtual void PureVirtualFunc() = 0;

        static ITest2Layer *CreateObject(int serialNo);
};

#endif

