#pragma once
#include "Types.h"

#include "ConflictData.h"
#include "CompressedBits.h"
#include "StrideDetector.h"
#include "dynamic_gcd.h"
#include "Dependence.h"

typedef CompressedData<Point> PointTable;
typedef CompressedData<Stride> StrideTable;

#define HAS_DEP_LIMIT 4

extern int64_t add_mem_time;

class LoopInstance {
    map<PC_ID,StrideDetector> detectors;
    PointTable pending_points;
    StrideTable pending_strides;

    PointTable history_points;
    StrideTable history_strides;

    CompressedSet killed_bits;
    access_mode_pair<CompressedSet> pending_bits;
    access_mode_pair<CompressedSet> history_bits;

    LoopInstanceDep my_dependencies;

    int64_t loop_count;
    int64_t has_dep_count;
    int64_t loop_id;
public:
    LoopInstance(int64_t in_loop_id);
    void addMemAccess(Block block,PC_ID identifier,StrideDetector & pc_detector);
    bool isKilled(Block block);
    void iteration_end();
    void merge_history_pending(LoopInstance & otherloop);

    const LoopInstanceDep &loop_end();
    int64_t get_loop_id(){return loop_id;}
protected:
    void handle_conflicts();

    void merge_pending_history();
};
