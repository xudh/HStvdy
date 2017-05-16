#ifndef __TEST_3LAYERA_H__
#define __TEST_3LAYERA_H__

#include "Test3Layer.h"

class CTest3LayerA : public CTest3Layer
{
public:
        CTest3LayerA();
        CTest3LayerA(int serialNo);
        ~CTest3LayerA();

        void VirtualFunc2();

        void DeepPureVirtualFunc();

public:
        void ClassInnerFunc();

private:
        void SomePriFunc();
};

#endif

