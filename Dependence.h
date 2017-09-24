#include __DEPENDENCE_H__
#define __DEPENDENCE_H__

enum DependenceType { NODEP = -1, RAW = 0, WAR = 1, WAW = 2 };

class Dependence {
  public:
    Dependence();

    Dependence(long long memoryAddr, long long earlierPC, MemAccessMode earlierAccessMode, long long laterPC,
               MemAccessMode laterAccessMode);

    DependenceType getDependenceType() { return myDependenceType; };

    long long getMemoryAddress() { return myMemoryAddr; };

  private:
    DependenceType myDependenceType;
    long long myMemoryAddr;

    long long myEarlierPC;
    MemAccessMode myEarlierAccessMode;

    long long myLaterPC;
    MemAccessMode myLaterAccessMode;
};

#endif
