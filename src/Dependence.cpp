#include "Dependence.h"

Dependence::Dependence() {
    myDependenceType = NODEP;
    myMemoryAddr = 0;
    myEarlierPC = myLaterPC = 0;
    myEarlierAccessMode = myLaterAccessMode = READ;
}

Dependence::Dependence(int64_t memoryAddr, int64_t earlierPC, MemAccessMode earlierAccessMode, int64_t laterPC,
                       MemAccessMode laterAccessMode) {
    myMemoryAddr = memoryAddr;
    myEarlierPC = earlierPC;
    myEarlierAccessMode = earlierAccessMode;
    myLaterPC = laterPC;
    myLaterAccessMode = laterAccessMode;

    if (earlierAccessMode == READ) {
        if (laterAccessMode == WRITE) {
            myDependenceType = RAW;
        }
    } else {
        if (laterAccessMode == READ) {
            myDependenceType = WAR;
        } else {
            myDependenceType = WAW;
        }
    }
}
