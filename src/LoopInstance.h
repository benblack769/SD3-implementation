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

typedef CompressedData<Point*> PointTable;
typedef CompressedData<Stride*> StrideTable;

class LoopData {
    map<PC_ID,StrideDetector> detectors;
    PointTable pending_points;
    StrideTable pending_strides;

    PointTable history_points;
    StrideTable history_strides;

    int64_t loop_count;

    void addMemAccess(Point point){
        //1. find overlap with killed bits of pending_tables using isKilled
        //2. if some overlap, then return without proceding
        //3. if new pc, add detector
        //4. add point to detetor
        //5. if detector released some points, add them to history tables (handleStrideDetectorResult)
    }
    bool isKilled(Point point){
        //1. construct PointTable with just the single item
        //2. Find overlap between this table and the pendingtables killed bits
        //3. return true if these overlaps have a containment of the point that is killed
    }
    void merge_history_pending(PointTable points, PointTable strides){
        //1. find 4-way overlap between points and strides
        //2. if killed bits don't completely overlap, then add point/stride to pendingtables
    }
    void iteration_end(){
        //1. increment loop_count
        //2. set all kill bits to false in pendingTables
        //3. merge_pending_history
        //4. add conflicts to table
    }
    pair<PointTable, StrideTable> stride_detector_info(){
        //1. go through strides, use purgeStrideDetectorState ideas o get info stored only in stridedetectors
        //2. return that info in new tables
    }
    void conflict_check_pending_history(PointTable points, StrideTable strides){
        //1. create new Point/Stride tables for detectors using stride_detector_info
        //2. find conflicts between them and pending points, strides
        //3. find conflicts between history tables and pending points, strides
        //4. add RAW conflicts to conflict table
    }
    void loop_end(){
        // must be called directly after iteration_end
        //1. flush stridedetectors out to history_tables by merging
    }
};


#endif
