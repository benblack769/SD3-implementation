#pragma once
#include "Types.h"
#include <list>

using namespace std;

public
ConflictTable {
  public:
    ConflictTable(int64_t startPC, int64_t endPC){};

    int getNumConflicts() { return myDependences.size(); };

  private:
    int64_t myLoopStartPC;
    int64_t myLoopEndPC;

    list<Dependence> myDependences;
};
