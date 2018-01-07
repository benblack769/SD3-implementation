#include "LoopInstance.h"
#include <ctime>

int64_t add_mem_time = 0;



LoopInstance::LoopInstance(int64_t in_loop_id){
    loop_id = in_loop_id;
    loop_count = 0;
}

void LoopInstance::addMemAccess(int64_t mem_addr, int64_t access_size, PC_ID identifier){
    CompressedSet new_set;
    new_set.add_block(mem_addr,access_size);
    //find overlap with killed bits of pending_tables using isKilled
    new_set.subtract(killed_bits);
    if(!new_set.any()){
        return;
    }
    //if is write and no reads before it, set killed bits
    if(identifier.get_acc_mode() == WRITE){
        CompressedSet new_killed_bits = new_set;
        new_killed_bits.subtract(pending_bits[READ].union_all());
        killed_bits.unite(new_killed_bits);
    }
    //add point to pending bit tables
    IntersectFinder & my_table = pending_bits[identifier.get_acc_mode()];
    my_table.add_new_set(identifier);
    my_table.add_values_to_key(identifier,new_set);
}
void LoopInstance::handle_all_conflicts(){
    handle_conflicts(WRITE,READ);
    handle_conflicts(READ,WRITE);
    handle_conflicts(WRITE,WRITE);
}
void LoopInstance::handle_conflicts(MemAccessMode pending_mode, MemAccessMode history_mode){
    CompressedSet intersect = pending_bits[pending_mode].union_all();
    intersect.intersect(history_bits[history_mode].union_all());
    int64_t conflict_count = intersect.count();
    LoopInstanceDep & cur_dependencies = my_dependencies[history_mode][pending_mode];
    if(conflict_count && cur_dependencies.conflict_iterations() <= HAS_DEP_LIMIT){
        vector<IntersectInfo> all_info = history_bits[history_mode].conflicting_keys(pending_bits[pending_mode]);
        vector<InstrDependence> out_dependencies(all_info.size());
        for(size_t i = 0; i < all_info.size(); i++){
            IntersectInfo info = all_info[i];
            out_dependencies[i] = InstrDependence(info.key_one,info.key_two,-1,info.size_of_intersect);
        }
        cur_dependencies.addIterationDependencies(out_dependencies,conflict_count);
    }
    else{
        cur_dependencies.addIterationDepsNoInstrs(conflict_count);
    }
}

void LoopInstance::merge_pending_history(){
    //merge pending into history
    int64_t start = my_clock();
    history_bits[READ].merge(pending_bits[READ]);
    history_bits[WRITE].merge(pending_bits[WRITE]);
    
    pending_bits[READ].clear();
    pending_bits[WRITE].clear();
    killed_bits.clear();
}
void LoopInstance::iteration_end(){
    loop_count++;
    
    int64_t start = my_clock();
    add_mem_time += my_clock() - start;
    handle_all_conflicts();
    merge_pending_history();
}
void LoopInstance::merge_history_pending(LoopInstance & otherloop){
    //merge in bits
    otherloop.history_bits[READ].subtract_from_all(killed_bits);
    otherloop.history_bits[WRITE].subtract_from_all(killed_bits);
    pending_bits[READ].merge(otherloop.history_bits[READ]);
    pending_bits[WRITE].merge(otherloop.history_bits[WRITE]);
    
    //add in new killed bits
    CompressedSet new_kill_bits = otherloop.history_bits[WRITE].union_all();
    new_kill_bits.subtract(pending_bits[READ].union_all()); 
    killed_bits.unite(new_kill_bits);
}
AllLoopInstanceDep &LoopInstance::loop_end(){
    return my_dependencies;
}
