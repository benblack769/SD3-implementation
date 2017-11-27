#pragma once
#include "Types.h"

#include "CompressedBits.h"
#include "FindIntersect.h"
#include "Dependence.h"

#define HAS_DEP_LIMIT 4

extern int64_t add_mem_time;

typedef access_mode_pair<access_mode_pair<LoopInstanceDep> > AllLoopInstanceDep;

class LoopInstance {
protected:
    CompressedSet killed_bits;
    access_mode_pair<IntersectFinder> pending_bits;
    access_mode_pair<IntersectFinder> history_bits;

    AllLoopInstanceDep my_dependencies;//history_acc_mode<pending_acc_mode<deps>>

    int64_t loop_count;
    int64_t loop_id;
public:
    LoopInstance(int64_t in_loop_id);
    void addMemAccess(int64_t mem_addr,int64_t access_size,PC_ID identifier);
    bool isKilled(int64_t mem_addr,int64_t access_size);
    void iteration_end();
    void merge_history_pending(LoopInstance & otherloop);

    AllLoopInstanceDep &loop_end();
    int64_t get_loop_id(){return loop_id;}
protected:
    void handle_all_conflicts();
    void handle_conflicts(MemAccessMode pending_mode,MemAccessMode history_mode);

    void merge_pending_history();
};
