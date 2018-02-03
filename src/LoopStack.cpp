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
    if(stack.size() > 0){
        int64_t start = my_clock();
        stack.back().addMemAccess(mem_addr,access_size,PC_ID(instr_address,acc_mode));
        add_timer += my_clock() - start;
    }
}

void add_all_summaries(AllLoopTotalSummary & summary, AllLoopInstanceDep & deps){
    summary[READ][WRITE].addLoopInstanceSummary(deps[READ][WRITE]);
    summary[WRITE][WRITE].addLoopInstanceSummary(deps[WRITE][WRITE]);
    summary[WRITE][READ].addLoopInstanceSummary(deps[WRITE][READ]);
}
void LoopStack::loop_end(int64_t loop_id){
    assert(stack.size() != 0);
    assert(stack.back().get_loop_id() == loop_id);
    int64_t start = my_clock();
    if(stack.size() > 1){
        second_from_top().merge_history_pending(stack.back());
    }
    add_all_summaries(loop_dependencies[loop_id],stack.back().loop_end());
    stack.pop_back();
    merge_timer += my_clock() - start;
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
    cout << "{\n";
    cout << "\"Timings\":" << endl;
    cout << "{\n\"TotalTime\": " << full_timer << ", \n";
    cout << "\"Add\": " << add_timer << ", \n";
    //cout << "LoopInstaceMarker: " << add_mem_time << endl;
    cout << "\"Merge\": " << merge_timer << ", \n";
    cout << "\"ItEnd\": " << it_end_timer << " \n";
    cout << "},\n";

    cout << "\"Loops\": {\n";
    for(dependence_iterator it = loop_dependencies.begin(); it != loop_dependencies.end(); ){
        int64_t lid = it->first;
        cout  << "\"" << hex << lid << dec << "\":{\n";
        cout << "\"RAW\": " << "";
        cout << it->second[WRITE][READ] << ",\n";
        cout << "\"WAR\": " << "";
        cout << it->second[READ][WRITE] << ",\n";
        cout << "\"WAW\": " << "";
        cout << it->second[WRITE][WRITE] << "\n";
        cout << "}";
        ++it;
        if (it != loop_dependencies.end()){
            cout << "," << "\n";
        }
    }
    cout << "}\n";
    cout << "}\n";
}
