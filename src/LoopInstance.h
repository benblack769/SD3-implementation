#ifndef __LOOPINSTANCE_H__
#define __LOOPINSTANCE_H__
#include "Types.h"

#include "HistoryPointTable.h"
#include "HistoryStrideTable.h"
#include "PendingPointTable.h"
#include "PendingStrideTable.h"

class LoopInstance {
  public:
    LoopInstance(int64_t startPC, int64_t endPC);
    ~LoopInstance(){};

    void endCurrentIteration();

    void addMemAccess(int64_t memAddress, int64_t PC, int accessSize = 4, MemAccessMode readOrWrite = READ);

    int64_t getStartPC() { return myLoopStartPC; };
    int64_t getEndPC() { return myLoopEndPC; };
    ConflictTable *getConflicts() { return myConflicts; };

  private:
    PendingPointTable *myPendingPoints;
    PendingStrideTable *myPendingStrides;
    HistoryPointTable *myHistoryPoints;
    HistoryStrideTable *myHistoryStrides;

    int myNumIterations;

    int64_t myLoopStartPC;
    int64_t myLoopEndPC;

    ConflictTable *myConflicts;

    void checkForPointDependences();

    void checkForStrideDepencences();

    void foldPendingPointsIntoHistory();

    void foldPendingStridesIntoHistory();

    // Stride detector for every PC
    struct PerPCDetector {
        StrideDetector *detector;
        MemAccessMode mode;
        MemAccessType lastType;

        PerPCDetector(MemAccessMode readOrWrite) {
            detector = new StrideDetector();
            mode = readOrWrite;
            lastType = UNDEFINED;
        }
    };

    // indexed by PC
    multimap<int64_t, PerPCDetector> myStrideDetectors;
};

#endif
