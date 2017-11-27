#include "LoopStack.h"
#include <ctime>
#include <iostream>

int64_t add_timer = 0;
int64_t merge_timer = 0;
int64_t it_end_timer = 0;
int64_t full_timer = 0;

LoopStack::LoopStack(){
    full_timer = my_clock();
}

void LoopStack::addMemAccess(int64_t mem_addr,int64_t access_size,int64_t instr_address,MemAccessMode acc_mode){
    int64_t start = my_clock();
    stack.back().addMemAccess(mem_addr,access_size,PC_ID(instr_address,acc_mode));
    add_timer += my_clock() - start;
}

void add_all_summaries(AllLoopTotalSummary & summary, AllLoopInstanceDep & deps){
    summary[READ][WRITE].addLoopInstanceSummary(deps[READ][WRITE]);
    summary[WRITE][WRITE].addLoopInstanceSummary(deps[WRITE][WRITE]);
    summary[WRITE][READ].addLoopInstanceSummary(deps[WRITE][READ]);
}
void LoopStack::loop_end(int64_t loop_id){
    assert(stack.size() != 0);
    assert(stack.back().get_loop_id() == loop_id);
    if(stack.size() > 1){
        int64_t start = my_clock();
        second_from_top().merge_history_pending(stack.back());
        merge_timer += my_clock() - start;
    }
    add_all_summaries(loop_dependencies[loop_id],stack.back().loop_end());
    stack.pop_back();
}
void LoopStack::loop_start(int64_t loop_id){
    stack.push_back(LoopInstance(loop_id));
}
void LoopStack::iter_end(int64_t loop_id){
    assert(stack.back().get_loop_id() == loop_id);
    int64_t start = my_clock();
    stack.back().iteration_end();
    it_end_timer += my_clock() - start;
}

LoopInstance & LoopStack::second_from_top(){
    assert(stack.size() > 1);
    return *(++stack.rbegin());
}
void LoopStack::print_loop_dependencies(){
    full_timer = my_clock() - full_timer;
    cout << "Timings:" << endl;
    cout << "Total time: " << full_timer << endl;
    cout << "Add: " << add_timer << endl;
    cout << "LoopInstaceMarker: " << add_mem_time << endl;
    cout << "Merge: " << merge_timer << endl;
    cout << "It end: " << it_end_timer << endl;
    
    for(dependence_iterator it = loop_dependencies.begin(); it != loop_dependencies.end(); it++){
        int64_t lid = it->first;
        cout << "LOOP " << lid << "\n";    
        cout << "RAW dependencies: " << "\n";
        cout << it->second[READ][WRITE] << endl;
        cout << "WAR dependencies: " << "\n";
        cout << it->second[WRITE][READ] << endl;
        cout << "WAW dependencies: " << "\n";
        cout << it->second[WRITE][WRITE] << endl;
    }
}

