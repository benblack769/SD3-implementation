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
    LoopStack(){
    }
    void addMemAccess(int64_t mem_addr,int64_t access_size,int64_t instr_address,MemAccessMode acc_mode){
        stack.back().addMemAccess(Block(mem_addr,mem_addr+access_size),PC_ID(instr_address,acc_mode),stride_detector[instr_address]);
    }
    void loop_end(int64_t loop_id){
        assert(stack.size() != 0);
        stack.back().loop_end(loop_dependencies[stack.back().get_loop_id()]);
        if(stack.size() > 1){
            second_from_top().merge_history_pending(stack.back());
        }
        stack.pop_back();
    }
    void loop_start(int64_t loop_id){
        stack.push_back(LoopInstance(loop_id,loop_instance_counts[loop_id]));
        loop_instance_counts[loop_id]++;
    }
    void iter_end(int64_t loop_id){
        assert(stack.back().get_loop_id() == loop_id);
        stack.back().iteration_end();
    }

    LoopInstance & second_from_top(){
        assert(stack.size() > 1);
        return *(++stack.rbegin());
    }
    void print_loop_dependencies(){
        for(dependence_iterator it = loop_dependencies.begin(); it != loop_dependencies.end(); it++){
            vector<Dependence> & cur_dep = it->second;
            int64_t lid = it->first;
            cout << "LOOP " << lid << "\n";
            for(size_t i = 0; i < cur_dep.size(); i++){
                cout << '\t' << cur_dep[i] << "\n";
            }
        }
    }
};
