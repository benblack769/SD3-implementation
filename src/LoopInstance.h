#pragma once
#include "Types.h"

#include "ConflictData.h"
#include "CompressedBits.h"
#include "StrideDetector.h"
#include "dynamic_gcd.h"
#include "Dependence.h"

typedef CompressedData<Point> PointTable;
typedef CompressedData<Stride> StrideTable;

class LoopInstance {
    map<PC_ID,StrideDetector> detectors;
    PointTable pending_points;
    StrideTable pending_strides;

    PointTable history_points;
    StrideTable history_strides;

    CompressedBits killed_bits;
    access_mode_pair<CompressedBits> pending_bits;

    access_mode_pair<CompressedBits> history_bits;

    vector<Dependence> my_dependencies;

    int64_t loop_count;
    int64_t loop_id;
    int64_t instance_num;
public:
    LoopInstance(int64_t in_loop_id,int64_t in_instance_num){
        loop_id = in_loop_id;
        instance_num = in_instance_num;
        loop_count = 0;
    }

    void addMemAccess(Block block,PC_ID identifier,StrideDetector & pc_detector){
        //find overlap with killed bits of pending_tables using isKilled
        if(this->isKilled(block)){
            return;
        }
        //add point to pending bit tables
        pending_bits[identifier.get_acc_mode()].add_block(block.begin(),block.length());
        //if is write and no reads before it, set killed bits
        if(identifier.get_pc() == WRITE && !pending_bits[READ].has_any_in_block(block.begin(),block.length())){
            killed_bits.add_block(block.begin(),block.length());
        }

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
    bool isKilled(Block block){
        return killed_bits.has_any_in_block(block.begin(),block.length());
    }
    bool pending_history_bits_conflict(){
        CompressedBits conflict_bits = pending_bits[READ];
        conflict_bits &= history_bits[WRITE];
        return conflict_bits.any();
    }
    void handle_conflicts(){
        //don't find conflicts if there aren't any
        if(pending_history_bits_conflict()){
            //1. find 4-way overlap between points and strides
            conflicts(history_points,pending_points,my_dependencies);
            conflicts(history_points,pending_strides,my_dependencies);
            conflicts(history_strides,pending_points,my_dependencies);
            conflicts(history_strides,pending_strides,my_dependencies);
        }
    }

    void merge_pending_history(){
        //merge pending into history
        history_bits[READ] |= pending_bits[READ];
        history_bits[WRITE] |= pending_bits[WRITE];

        history_points.merge_into(pending_points);
        history_strides.merge_into(pending_strides);

        pending_bits[READ].clear();
        pending_bits[WRITE].clear();
        killed_bits.clear();

        pending_points.clear();
        pending_strides.clear();
    }
    void iteration_end(){
        loop_count++;
        handle_conflicts();
        merge_pending_history();
    }
    void merge_history_pending(LoopInstance & otherloop){
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
        pending_bits[READ] |= otherloop.history_bits[READ];
        pending_bits[WRITE] |= otherloop.history_bits[WRITE];
    }

    void loop_end(vector<Dependence> & out_loop_dependences){
        out_loop_dependences.insert(out_loop_dependences.begin(),my_dependencies.begin(),my_dependencies.end());
    }
    int64_t get_loop_id(){return loop_id;}

    template<class IntTy1,class IntTy2>
    void only_conflicts(vector<pair<IntTy1,IntTy2> > & in_overlap,vector<Dependence> & out_conflicts){
        for(size_t i = 0; i < in_overlap.size(); i++){
            pair<IntTy1,IntTy2> cur_data = in_overlap[i];
            if(has_overlap(cur_data.first,cur_data.second)){
                //only gets an approximate memory address for simplicity
                out_conflicts.push_back(Dependence(cur_data.first.getPC_ID(),cur_data.second.getPC_ID(),cur_data.first.first(),this->loop_count,this->instance_num));
            }
        }
    }
    template<class IntTy1,class IntTy2>
    void conflicts(CompressedData<IntTy1> & first,CompressedData<IntTy2> & second,vector<Dependence> & out_conflicts){
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
};
