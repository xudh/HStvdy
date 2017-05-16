#ifndef __INTERFACE_TEST_3LAYER_H__
#define __INTERFACE_TEST_3LAYER_H__

class ITest3Layer
{
protected:
        ITest3Layer();

public:
        virtual ~ITest3Layer();

        virtual void VirtualFunc();                                     //  这两函数在2、3层都实现
        virtual void PureVirtualFunc() = 0;

        virtual void VirtualFunc2();                            //  这两函数在2层干扰、3层实现
        virtual void PureVirtualFunc2() = 0;

        virtual void ShallowVirtualFunc();                      //  这两函数只在2层实现
        virtual void ShallowPureVirtualFunc() = 0;

        virtual void DeepVirtualFunc();                         //  这两函数只在3层实现
        virtual void DeepPureVirtualFunc() = 0;

        static ITest3Layer *CreateObject(int serialNo);
};

#endif

