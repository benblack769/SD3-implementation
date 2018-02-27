#include "LoopInstance.h"
#include <ctime>

int64_t add_mem_time = 0;



LoopInstance::LoopInstance(int64_t in_loop_id){
    loop_id = in_loop_id;
    loop_count = 0;
}

void LoopInstance::addMemAccess(int64_t mem_addr, int64_t access_size, PC_ID identifier){
    pending_bits[identifier.get_acc_mode()].add_block(identifier,mem_addr,access_size,identifier.get_acc_mode() == READ, pending_bits[READ], pending_bits[WRITE]);
}
void LoopInstance::handle_all_conflicts(){
    handle_conflicts(WRITE,READ);
    handle_conflicts(READ,WRITE);
    handle_conflicts(WRITE,WRITE);
}
void LoopInstance::handle_conflicts(MemAccessMode pending_mode, MemAccessMode history_mode){
    LoopInstanceDep & cur_dependencies = my_dependencies[history_mode][pending_mode];
    //if(all_info.size()){// && cur_dependencies.conflict_iterations() <= HAS_DEP_LIMIT){
    vector<IntersectInfo> all_info = history_bits[history_mode].conflicting_keys(pending_bits[pending_mode]);
    int64_t total_count = 0;
    vector<InstrDependence> out_dependencies(all_info.size());
    for(size_t i = 0; i < all_info.size(); i++){
        IntersectInfo info = all_info[i];
        total_count += info.size_of_intersect;
        out_dependencies[i] = InstrDependence(info.key_one,info.key_two,info.approx_mem_addr,info.size_of_intersect);
    }
    cur_dependencies.addIterationDependencies(out_dependencies,total_count);
}

void LoopInstance::merge_pending_history(){
    //merge pending into history
    history_bits[READ].merge(pending_bits[READ]);
    history_bits[WRITE].merge(pending_bits[WRITE]);
    
    pending_bits[READ].clear();
    pending_bits[WRITE].clear();
}
void LoopInstance::iteration_end(){
    loop_count++;
    handle_all_conflicts();
    merge_pending_history();
}
void LoopInstance::merge_history_pending(LoopInstance & otherloop){
    //merge in bits
    otherloop.history_bits[READ].subtract_from_all(pending_bits[READ], pending_bits[WRITE]);
    otherloop.history_bits[WRITE].subtract_from_all(pending_bits[READ], pending_bits[WRITE]);
    pending_bits[READ].merge(otherloop.history_bits[READ]);
    pending_bits[WRITE].merge(otherloop.history_bits[WRITE]);
}
AllLoopInstanceDep &LoopInstance::loop_end(){
    return my_dependencies;
}
