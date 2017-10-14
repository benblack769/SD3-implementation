#include __DEPENDENCE_H__
#define __DEPENDENCE_H__
#include "Types.h"

enum DependenceType { NODEP = -1, RAW = 0, WAR = 1, WAW = 2 };

class Dependence {
  public:
    Dependence();

    Dependence(int64_t memoryAddr, int64_t earlierPC, MemAccessMode earlierAccessMode, int64_t laterPC,
               MemAccessMode laterAccessMode);

    DependenceType getDependenceType() { return myDependenceType; };

    int64_t getMemoryAddress() { return myMemoryAddr; };

  private:
    DependenceType myDependenceType;
    int64_t myMemoryAddr;

    int64_t myEarlierPC;
    MemAccessMode myEarlierAccessMode;

    int64_t myLaterPC;
    MemAccessMode myLaterAccessMode;
};

#endif
