#include __CONFLICTTABLE_H__
#define __CONFLICTTABLE_H__

#include <list>
#include "Types.h"

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

#endif
