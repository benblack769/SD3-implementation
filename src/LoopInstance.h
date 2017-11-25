#pragma once
#include "Types.h"

#include "ConflictData.h"
#include "CompressedBits.h"
#include "Dependence.h"

#define HAS_DEP_LIMIT 4

extern int64_t add_mem_time;

typedef access_mode_pair<access_mode_pair<LoopInstanceDep> > AllLoopInstanceDep;

class LoopInstance {
protected:
    map<PC_ID,StrideDetector> detectors;
    CompressedSet killed_bits;
    access_mode_pair<CompressedSet> pending_bits;
    access_mode_pair<CompressedSet> history_bits;

    AllLoopInstanceDep my_dependencies;//history_acc_mode<pending_acc_mode<deps>>

    int64_t loop_count;
    int64_t loop_id;
public:
    LoopInstance(int64_t in_loop_id);
    void addMemAccess(Block block,PC_ID identifier,StrideDetector & pc_detector);
    bool isKilled(Block block);
    void iteration_end();
    void merge_history_pending(LoopInstance & otherloop);

    AllLoopInstanceDep &loop_end();
    int64_t get_loop_id(){return loop_id;}
protected:
    void handle_all_conflicts();
    void handle_conflicts(MemAccessMode pending_mode,MemAccessMode history_mode);

    void merge_pending_history();
};
