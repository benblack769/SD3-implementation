#include "Dependence.h"

Dependence::Dependence()
{
  myDependenceType = NODEP;
  myMemoryAddr = 0;
  myEarlierPC = myLaterPC = 0;
  myEarlierAccessMode = myLaterAccessMode = READ;
}

Dependence::Dependence(long long memoryAddr, long long earlierPC, MemAccessMode earlierAccessMode, long long laterPC, MemAccessMode laterAccessMode)
{
  myMemoryAddr = memoryAddr;
  myEarlierPC = earlierPC;
  myEarlierAccessMode = earlierAccessMode;
  myLaterPC = laterPC;
  myLaterAccessMode = laterAccessMode;

  if(earlierAccessMode == READ){
    if(laterAccessMode == WRITE){
      myDependenceType = RAW;
    }
  }
  else{
    if(laterAccessMode == READ){
      myDependenceType = WAR;
    }
    else{
      myDependenceType = WAW;
    }
  }
}
