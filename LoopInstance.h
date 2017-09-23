#ifndef __LOOPINSTANCE_H__
#define __LOOPINSTANCE_H__

#include "PendingPointTable.h"
#include "PendingStrideTable.h"
#include "HistoryPointTable.h"
#include "HistoryStrideTable.h"

class LoopInstance
{
 public:
  LoopInstance(long long startPC, long long endPC);
  ~LoopInstance(){};

  void endCurrentIteration();
  
  void addMemAccess(long long memAddress, long long PC, int accessSize=4, MemAccessMode readOrWrite=READ);

  long long getStartPC(){ return myLoopStartPC; };
  long long getEndPC() { return myLoopEndPC; };
  ConflictTable * getConflicts() { return myConflicts; };

 private:
  PendingPointTable *myPendingPoints;
  PendingStrideTable *myPendingStrides;
  HistoryPointTable *myHistoryPoints;
  HistoryStrideTable *myHistoryStrides;

  int myNumIterations;

  long long myLoopStartPC;
  long long myLoopEndPC;

  ConflictTable *myConflicts;

  void checkForPointDependences();

  void checkForStrideDepencences();

  void foldPendingPointsIntoHistory();

  void foldPendingStridesIntoHistory();

  // Stride detector for every PC
  struct PerPCDetector{
    StrideDetector *detector;
    MemAccessMode mode;
    MemAccessType lastType;
    
    PerPCDetector(MemAccessMode readOrWrite){
      detector = new StrideDetector();
      mode = readOrWrite;
      lastType = UNDEFINED;
    }
  };
      
  // indexed by PC
  multimap<long long, PerPCDetector> myStrideDetectors;
};

#endif
