#include "LoopStack.h"

void LoopStack::addMemAccess(int64_t mem_addr,int64_t access_size,int64_t instr_address,MemAccessMode acc_mode){
    stack.back().addMemAccess(Block(mem_addr,mem_addr+access_size),PC_ID(instr_address,acc_mode),stride_detector[instr_address]);
}
void LoopStack::loop_end(int64_t loop_id){
    assert(stack.size() != 0);
    stack.back().loop_end(loop_dependencies[stack.back().get_loop_id()]);
    if(stack.size() > 1){
        second_from_top().merge_history_pending(stack.back());
    }
    stack.pop_back();
}
void LoopStack::loop_start(int64_t loop_id){
    stack.push_back(LoopInstance(loop_id,loop_instance_counts[loop_id]));
    loop_instance_counts[loop_id]++;
}
void LoopStack::iter_end(int64_t loop_id){
    assert(stack.back().get_loop_id() == loop_id);
    stack.back().iteration_end();
}

LoopInstance & LoopStack::second_from_top(){
    assert(stack.size() > 1);
    return *(++stack.rbegin());
}
void LoopStack::print_loop_dependencies(){
    for(dependence_iterator it = loop_dependencies.begin(); it != loop_dependencies.end(); it++){
        vector<Dependence> & cur_dep = it->second;
        int64_t lid = it->first;
        cout << "LOOP " << lid << "\n";
        for(size_t i = 0; i < cur_dep.size(); i++){
            cout << '\t' << cur_dep[i] << "\n";
        }
    }
}
