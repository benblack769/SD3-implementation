#pragma once
#include "Types.h"

#include "ConflictData.h"
#include "CompressedBits.h"
#include "StrideDetector.h"
#include "dynamic_gcd.h"
#include "Dependence.h"

typedef CompressedData<Point> PointTable;
typedef CompressedData<Stride> StrideTable;

class LoopInstance {
    map<PC_ID,StrideDetector> detectors;
    PointTable pending_points;
    StrideTable pending_strides;

    PointTable history_points;
    StrideTable history_strides;

    CompressedBits killed_bits;
    access_mode_pair<CompressedBits> pending_bits;

    access_mode_pair<CompressedBits> history_bits;

    vector<Dependence> my_dependencies;

    int64_t loop_count;
    int64_t loop_id;
    int64_t instance_num;
public:
    LoopInstance(int64_t in_loop_id,int64_t in_instance_num);
    void addMemAccess(Block block,PC_ID identifier,StrideDetector & pc_detector);
    bool isKilled(Block block);
    void iteration_end();
    void merge_history_pending(LoopInstance & otherloop);

    void loop_end(vector<Dependence> & out_loop_dependences);
    int64_t get_loop_id(){return loop_id;}
protected:
    bool pending_history_bits_conflict();
    void handle_conflicts();

    void merge_pending_history();
};
