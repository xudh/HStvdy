#ifndef __TEST_3LAYER_H__
#define __TEST_3LAYER_H__

#include "VirtualTest/Test3Layer.h"

class CTest3Layer : public ITest3Layer
{
//public:                        // 因为暗含纯虚函数DeepPureVirtualFunc()，所以不能创建对象
protected:
        CTest3Layer();
        virtual ~CTest3Layer();

private:
        void VirtualFunc();             //  前面加不加virtual无所谓，因为不能创建对象
        void PureVirtualFunc();

        void ShallowVirtualFunc();
        void ShallowPureVirtualFunc();

// 篡改这两个函数
public:
        virtual void VirtualFunc2() = 0;
private:        // 继续派生继承的是它，而非它的基类，就算private也不例外
        virtual void PureVirtualFunc2();

public: // 正是因为这块存在，必须再派生一层类
        virtual void OtherVirtualFunc();
        static void CommonInnerFunc();

private:
        void SomePriFunc();

};

#endif

