#pragma once
#include "dynamic_gcd.h"
#include "IntervalOverlap.h"
#include "Point.h"
#include "Dependence.h"
#define unordered_map map

template<class IntervalTy>
class PC_Data{
    //container for points of the same PC_ID
    //SHOULD ONLY BE USED FOR POINTS!
protected:
    //assuming basic "contains" based merge, strides and points
    //that start on the same memory and have the same PC are unique
    unordered_map<int64_t,IntervalTy> intervals;//unordered_map
    typedef typename unordered_map<int64_t,IntervalTy>::iterator map_it_ty;
public:
    void add(IntervalTy interval){
        intervals[interval.first()] = interval;
    }
    void merge_into(PC_Data<IntervalTy> & from){
        //merge if and only if they start at the same mem_address (good for points)
        for(map_it_ty it = from.intervals.begin(); it != from.intervals.end(); it++){
            int64_t key = it->first;
            this->intervals[key] = it->second;
        }
    }
    void append_to(vector<IntervalTy> & cur_data){
        for(map_it_ty it = intervals.begin(); it != intervals.end(); it++){
            cur_data.push_back(it->second);
        }
    }
};

inline Stride merge_strides(Stride one, Stride other){
    assert(one.getPC_ID() == other.getPC_ID());
    return Stride(merge(one,other),one.getPC_ID());
}
class len_offset{
public:
    len_offset(int64_t stride_start,int64_t stride_len):
        len(stride_len),offset(stride_start%stride_len){
        assert(stride_start >= 0 && stride_len > 0);
    }
    len_offset():len(-1),offset(-1){}
    bool operator < (len_offset other)const{
        return this->len < other.len ||  (this->len == other.len && this->offset < other.offset);
    }
    bool operator == (len_offset other)const{
        return this->len == other.len &&  this->len == other.len;
    }
protected:
    int64_t len;
    int64_t offset;
};

template<>
class PC_Data<Stride>{
    /*
    This container is optimized for perfect stride merging (assumes all strides have same PC).

    All strides added to this container are merged with high accuracy
    The 3 requirements for stride merging here are:

        1. Same stride length
        2. Offset between strides is divisible by the stride length
        3. Lengths accessed adjoin (will form a complete stride without gaps if merged)

    This is implemented efficiently be grouping strides that meet requirements 1 and 2
    in the same bucket of a hash table. Then condition 3 can be easily checking in
    an ordinary BST.
    */
protected:
    typedef map<int64_t,Stride> mem_addr_map;
    typedef unordered_map<len_offset,mem_addr_map> offset_map;
    offset_map intervals;
    typedef typename offset_map::iterator offset_it;
    typedef typename mem_addr_map::iterator mem_addr_it;
public:
    void add(Stride interval){
        len_offset key(interval.first(),interval.stride());
        merge_single_item(intervals[key],interval);
    }
    void merge_into(PC_Data<Stride> & from){
        for(offset_it s_it = from.intervals.begin(); s_it != from.intervals.end(); s_it++){
            len_offset key = s_it->first;
            if(this->intervals.count(key)){
                this->merge_same_stride_len_offset(this->intervals[key], s_it->second);
            }
            else{
                this->intervals[key] = s_it->second;
            }
        }
    }
    void append_to(vector<Stride> & cur_data){
        for(offset_it o_it = intervals.begin(); o_it != intervals.end(); o_it++){
            mem_addr_map & mem_addrs = o_it->second;
            for(mem_addr_it m_it = mem_addrs.begin(); m_it != mem_addrs.end(); m_it++){
                cur_data.push_back(m_it->second);
            }
        }
    }
protected:
    void merge_single_item(mem_addr_map & into, Stride merge_stride){
        int64_t key = merge_stride.first();
        if(into.size() == 0){
            into[key] = merge_stride;
            return;
        }

        mem_addr_it greater = into.upper_bound(key);

        //only need to check the one interval below merge_stride.first(), because
        //if strides below that could have been merged, then they already would have been
        if(greater != into.begin()){
            mem_addr_it lower_eq_it = greater;
            --lower_eq_it;

            Stride lower_eq_stride = lower_eq_it->second;
            //merge in lower or equal vals simply by extending last(), no changes to map
            if(lower_eq_stride.end() >= merge_stride.first()){
                lower_eq_it->second = merge_strides(lower_eq_stride,merge_stride);
            }
        }
        //checks every stride above it for merging
        for(;greater != into.end() && merge_stride.end() >= greater->second.first(); greater++){
            Stride greater_stride = greater->second;
            //need to change map key (first()), so remove old value from map, then add
            into.erase(greater);
            Stride merged_stride = merge_strides(merge_stride,greater_stride);
            into[merged_stride.first()] = merged_stride;
        }
    }

    void merge_same_stride_len_offset(mem_addr_map & into, mem_addr_map & from){
        if(into.size() == 0){
            into = from;
            return;
        }
        for(mem_addr_it m_it = from.begin(); m_it != from.end(); m_it++){
            Stride merge_stride = m_it->second;
            merge_single_item(into,merge_stride);
        }
    }
};


template<class IntervalTy>
class CompressedData {
    /*
    StrideTable and PointTable, in same interface.
    */
protected:
    typedef typename vector<IntervalTy>::iterator vec_iterator;
    typedef PC_Data<IntervalTy> PC_Data_ty;
    unordered_map<PC_ID,PC_Data_ty> pc_table;//unordered_map
    typedef typename unordered_map<PC_ID,PC_Data_ty>::iterator pc_table_iterator;
public:
    void add(IntervalTy interval){
        pc_table[interval.getPC_ID()].add(interval);
    }
    void merge_into(CompressedData<IntervalTy> & from){
        for(pc_table_iterator it = from.pc_table.begin(); it != from.pc_table.end(); ++it){
            PC_ID id = it->first;
            PC_Data_ty & from_pc_data = it->second;
            this->pc_table[id].merge_into(from_pc_data);
        }
    }
    void clear(){
        pc_table.clear();
    }
    void print(){
        vector<IntervalTy> sorted_vals;
        intervals(sorted_vals);
        sort_by_first(sorted_vals);
        for(vec_iterator it = sorted_vals.begin(); it != sorted_vals.end(); it++){
            cout << *it << endl;
        }
    }
    void intervals(vector<IntervalTy> & out_intervals,bool filter = false,MemAccessMode mode = WRITE){
        for(pc_table_iterator it = pc_table.begin(); it != pc_table.end(); ++it){
            if(!(filter && it->first.get_acc_mode() == mode)){
                PC_Data_ty & pc_data = it->second;
                pc_data.append_to(out_intervals);
            }
        }
    }
};
template<class IntTy1,class IntTy2>
void only_conflicts(vector<pair<IntTy1,IntTy2> > & in_overlap,vector<Dependence> & out_conflicts){
    for(size_t i = 0; i < in_overlap.size(); i++){
        pair<IntTy1,IntTy2> cur_data = in_overlap[i];
        if(has_overlap(cur_data.first,cur_data.second)){
            //only gets an approximate memory address for simplicity
            out_conflicts.push_back(Dependence(cur_data.first.getPC_ID(),cur_data.second.getPC_ID(),cur_data.first.first()));
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
