#include "LoopInstance.h"
#include <ctime>

int64_t add_mem_time = 0;

LoopInstance::LoopInstance(int64_t in_loop_id){
    loop_id = in_loop_id;
    loop_count = 0;
}

void LoopInstance::addMemAccess(int64_t mem_addr,int64_t access_size,int64_t instr_address,MemAccessMode acc_mode){
    if(isKilled(mem_addr)){
        return;
    }
    pending_table[acc_mode].add(instr_address,mem_addr,access_size);
}
void LoopInstance::handle_all_conflicts(){
    handle_conflicts(READ,WRITE);
    handle_conflicts(WRITE,READ);
    handle_conflicts(WRITE,WRITE);
}
void LoopInstance::handle_conflicts(MemAccessMode pending_mode, MemAccessMode history_mode){
    LoopInstanceDep & cur_dependencies = my_dependencies[history_mode][pending_mode];
    if(cur_dependencies.conflict_iterations() < HAS_DEP_LIMIT){
        vector<InstrDependence> conflicts;
        int64_t conflict_byte_count = 0;
        history_table[history_mode].conflicts(pending_table[pending_mode],history_mode,pending_mode,conflicts,conflict_byte_count);
        assert((conflict_byte_count == 0) == (conflicts.size() == 0));
        if(conflicts.size()){
            cur_dependencies.addIterationDependencies(conflicts,conflict_byte_count);
        }
        else{
            cur_dependencies.addIterationDepsNoInstrs(0);
        }
    }
    else{
        cur_dependencies.addIterationDepsNoInstrs(0);
    }
}
bool LoopInstance::isKilled(int64_t mem_addr){
    bool has_read_before = pending_table[READ].has(mem_addr);
    bool has_write_before = pending_table[WRITE].has(mem_addr);
    return !has_read_before && has_write_before;
}
void LoopInstance::add_in_table(CompressedData & into, CompressedData & from){
    for(CompressedData::table_iterator it = from.mem_addr_table.begin(); it != from.mem_addr_table.end(); ++it){
        int64_t mem_addr = it->first;
        if(!isKilled(mem_addr)){
            into.add_pointlist(mem_addr, it->second);
        }
    }
}
void LoopInstance::merge_pending_history(){
    //merge pending into history
    history_table[READ].merge_into(pending_table[READ]);
    history_table[WRITE].merge_into(pending_table[WRITE]);

    pending_table[READ].clear();
    pending_table[WRITE].clear();
}
void LoopInstance::iteration_end(){
    loop_count++;
    handle_all_conflicts();
    merge_pending_history();
}
void LoopInstance::merge_history_pending(LoopInstance & otherloop){
    add_in_table(pending_table[READ],otherloop.history_table[READ]);
    add_in_table(pending_table[WRITE],otherloop.history_table[WRITE]);
}
AllLoopInstanceDep &LoopInstance::loop_end(){
    return my_dependencies;
}
