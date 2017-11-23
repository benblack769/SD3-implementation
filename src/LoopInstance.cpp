#include "LoopInstance.h"
#include <ctime>

int64_t add_mem_time = 0;

typedef CompressedData<Point> PointTable;
typedef CompressedData<Stride> StrideTable;


LoopInstance::LoopInstance(int64_t in_loop_id){
    loop_id = in_loop_id;
    loop_count = 0;
}

void LoopInstance::addMemAccess(Block block,PC_ID identifier,StrideDetector & pc_detector){
    //find overlap with killed bits of pending_tables using isKilled
    if(this->isKilled(block)){
        return;
    }
    //if is write and no reads before it, set killed bits
    if(identifier.get_acc_mode() == WRITE && !pending_bits[READ].has_any_in_block(block.begin(),block.length())){
        killed_bits.add_block(block.begin(),block.length());
    }
    //add point to pending bit tables
    pending_bits[identifier.get_acc_mode()].add_block(block.begin(),block.length());    
    
    //get pc detector result and add point/stride to tables
    int64_t mem_addr = block.begin();
    int64_t access_size = block.length();
    MemAccessType access_ty = pc_detector.addAccess(mem_addr);
    if(access_ty == POINT){
        Block access_block(mem_addr,mem_addr+access_size);
        pending_points.add(Point(access_block,identifier));
    }
    else{
        int64_t stride = pc_detector.getStride();
        SparseStride access_stride(mem_addr,1,stride,access_size);
        pending_strides.add(Stride(access_stride,identifier));
    }
}
bool LoopInstance::isKilled(Block block){
    return killed_bits.has_any_in_block(block.begin(),block.length());
}
template<class IntTy1,class IntTy2>
void only_conflicts(vector<pair<IntTy1,IntTy2> > & in_overlap,vector<InstrDependence> & out_conflicts){
    for(size_t i = 0; i < in_overlap.size(); i++){
        pair<IntTy1,IntTy2> cur_data = in_overlap[i];
        if(has_overlap(cur_data.first,cur_data.second)){
            //only gets an approximate memory address for simplicity
            int64_t num_mem_conflicts = 1;//simplify this process for now. Wrong, though
            out_conflicts.push_back(InstrDependence(cur_data.first.getPC_ID(),cur_data.second.getPC_ID(),cur_data.first.first(),num_mem_conflicts));
        }
    }
}
template<class IntTy1,class IntTy2>
void conflicts(CompressedData<IntTy1> & first,CompressedData<IntTy2> & second,vector<InstrDependence> & out_conflicts){
    vector<pair<IntTy1,IntTy2> > overlap;
    vector<IntTy1> first_inters;
    vector<IntTy2> second_inters;
    first.intervals(first_inters,true,WRITE);
    second.intervals(second_inters,true,READ);
    sort_by_first(first_inters);
    sort_by_first(second_inters);
    check_overlap_sorted(first_inters,second_inters,overlap);
    only_conflicts(overlap,out_conflicts);
}
void LoopInstance::handle_all_conflicts(){
    handle_conflicts(READ,WRITE);
    handle_conflicts(WRITE,READ);
    handle_conflicts(WRITE,WRITE);
}
void LoopInstance::handle_conflicts(MemAccessMode pending_mode, MemAccessMode history_mode){
    CompressedSet conflict_bits = pending_bits[pending_mode];
    conflict_bits.intersect(history_bits[history_mode]);
    LoopInstanceDep & cur_dependencies = my_dependencies[history_mode][pending_mode];
    if(conflict_bits.any()){
        int64_t conflict_count = conflict_bits.count();
        if(cur_dependencies.conflict_iterations() > HAS_DEP_LIMIT){
            cur_dependencies.addIterationDepsNoInstrs(conflict_count);
        }
        else{
            vector<InstrDependence> out_dependencies;
            //find 4-way overlap between points and strides
            conflicts(history_points,pending_points,out_dependencies);
            conflicts(history_points,pending_strides,out_dependencies);
            conflicts(history_strides,pending_points,out_dependencies);
            conflicts(history_strides,pending_strides,out_dependencies);
            
            cur_dependencies.addIterationDependencies(out_dependencies,conflict_count);
        }
    }
    else{
        cur_dependencies.addIterationDepsNoInstrs(0);
    }
}

void LoopInstance::merge_pending_history(){
    //merge pending into history
    int64_t start = my_clock();
    history_bits[READ].unite(pending_bits[READ]);
    history_bits[WRITE].unite(pending_bits[WRITE]);
    
    history_points.merge_into(pending_points);
    history_strides.merge_into(pending_strides);
    add_mem_time += my_clock() - start;
    
    pending_bits[READ].clear();
    pending_bits[WRITE].clear();
    killed_bits.clear();
    
    pending_points.clear();
    pending_strides.clear();
}
void LoopInstance::iteration_end(){
    loop_count++;
    handle_all_conflicts();
    merge_pending_history();
}
void LoopInstance::merge_history_pending(LoopInstance & otherloop){
    vector<Stride> in_strides;
    vector<Point> in_points;
    otherloop.history_strides.intervals(in_strides);
    otherloop.history_points.intervals(in_points);
    
    
    //merge in points
    for(size_t i = 0; i < in_points.size(); i++){
        Point this_point = in_points[i];
        if(!isKilled(this_point)){
            pending_points.add(this_point);
        }
    }
    //merge in strides
    for(size_t i = 0; i < in_strides.size(); i++){
        Stride this_stride = in_strides[i];
        //iterate over blocks until block is not killed
        int64_t block_point = this_stride.first();
        int64_t stride_size = this_stride.size();
        for(; block_point < this_stride.end() &&
            isKilled(Block(block_point,block_point+this_stride.block_size()));
            block_point += this_stride.stride(), stride_size--);
        //if any of the stride is left, add it back to the table
        if(block_point < this_stride.end()){
            SparseStride shortened(block_point,stride_size,this_stride.stride(),this_stride.block_size());
            pending_strides.add(Stride(shortened,this_stride.getPC_ID()));
        }
    }
    //merge in bits
    otherloop.history_bits[READ].subtract(killed_bits);
    otherloop.history_bits[WRITE].subtract(killed_bits);
    pending_bits[READ].unite(otherloop.history_bits[READ]);
    pending_bits[WRITE].unite(otherloop.history_bits[WRITE]);
    
    //add in new killed bits
    CompressedSet new_kill_bits = otherloop.history_bits[WRITE];
    new_kill_bits.subtract(pending_bits[READ]); 
    killed_bits.unite(new_kill_bits);
}
AllLoopInstanceDep &LoopInstance::loop_end(){
    return my_dependencies;
}
