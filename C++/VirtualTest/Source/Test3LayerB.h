#ifndef __TEST_3LAYERB_H__
#define __TEST_3LAYERB_H__

#include "Test3Layer.h"

class CTest3LayerB : public CTest3Layer
{
public:
        CTest3LayerB();
        CTest3LayerB(int serialNo);
        ~CTest3LayerB();

        void VirtualFunc();
        void PureVirtualFunc();

        void VirtualFunc2();
        void PureVirtualFunc2();

        void DeepVirtualFunc();
        void DeepPureVirtualFunc();

public:
        void ClassInnerFunc();

private:
        void SomePriFunc();
};

#endif

