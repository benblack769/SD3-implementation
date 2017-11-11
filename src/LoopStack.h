#pragma once
#include "LoopInstance.h"
#include <cassert>
#include <list>
#include "Dependence.h"

using namespace std;

class LoopStack{
protected:
    list<LoopInstance> stack;
    unordered_map<int64_t, vector<Dependence> > loop_dependencies;
    unordered_map<int64_t, int64_t > loop_instance_counts;
    typedef typename unordered_map<int64_t, vector<Dependence> >::iterator dependence_iterator;
    unordered_map<int64_t, StrideDetector> stride_detector;
public:
    LoopStack();
    void addMemAccess(int64_t mem_addr,int64_t access_size,int64_t instr_address,MemAccessMode acc_mode);
    void loop_end(int64_t loop_id);
    void loop_start(int64_t loop_id);
    void iter_end(int64_t loop_id);
    LoopInstance & second_from_top();
    void print_loop_dependencies();
};
