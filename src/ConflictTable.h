#include __CONFLICTTABLE_H__
#define __CONFLICTTABLE_H__

#include <list>

using namespace std;

public
ConflictTable {
  public:
    ConflictTable(long long startPC, long long endPC){};

    int getNumConflicts() { return myDependences.size(); };

  private:
    long long myLoopStartPC;
    long long myLoopEndPC;

    list<Dependence> myDependences;
};

#endif
