#include "LoopStack.h"
#include <ctime>

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
        stack.back().addMemAccess(Block(mem_addr,mem_addr+access_size),PC_ID(instr_address,acc_mode),stride_detector[instr_address]);
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
void LoopStack::print_loop_dependencies(ostream & os){
    full_timer = my_clock() - full_timer;
    os << "{\n";
    os << "\"Timings\":" << endl;
    os << "{\n\"TotalTime\": " << full_timer << ", \n";
    os << "\"Add\": " << add_timer << ", \n";
    //os << "LoopInstaceMarker: " << add_mem_time << endl;
    os << "\"Merge\": " << merge_timer << ", \n";
    os << "\"ItEnd\": " << it_end_timer << " \n";
    os << "},\n";

    os << "\"Loops\": {\n";
    for(dependence_iterator it = loop_dependencies.begin(); it != loop_dependencies.end(); ){
        int64_t lid = it->first;
        os  << "\"" << hex << lid << dec << "\":{\n";
        os << "\"RAW\": " << "";
        os << it->second[WRITE][READ] << ",\n";
        os << "\"WAR\": " << "";
        os << it->second[READ][WRITE] << ",\n";
        os << "\"WAW\": " << "";
        os << it->second[WRITE][WRITE] << "\n";
        os << "}";
        ++it;
        if (it != loop_dependencies.end()){
            os << "," << "\n";
        }
    }
    os << "}\n";
    os << "}\n";
}
