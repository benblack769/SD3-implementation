#pragma once
#include "Dependence.h"
#include "PC_ID.h"

class CompressedData {
protected:
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
};
