#pragma once
#include <vector>
#include "Point.h"
#include "PC_ID.h"
#include "Dependence.h"

class CompressedData {
public:
    unordered_map<int64_t,PointList> mem_addr_table;//unordered_map
    typedef typename unordered_map<int64_t,PointList>::iterator table_iterator;
    void add(int64_t pc, int64_t mem_addr,int64_t access_size){
        mem_addr_table[mem_addr].add(pc,access_size);
    }
    void add_pointlist(int64_t mem_addr,PointList & other){
        if(mem_addr_table.count(mem_addr)){
            mem_addr_table[mem_addr].merge_into(other);
        }
        else{
            mem_addr_table[mem_addr] = other;
        }
    }
    void merge_into(CompressedData & from){
        for(table_iterator it = from.mem_addr_table.begin(); it != from.mem_addr_table.end(); ++it){
            int64_t mem_addr = it->first;
            PointList & from_pc_data = it->second;
            add_pointlist(mem_addr,from_pc_data);
            //this->mem_addr_table[mem_addr].merge_into(from_pc_data);
        }
    }
    bool has(int64_t addr){
        return mem_addr_table.count(addr) != 0;
    }
    void clear(){
        mem_addr_table.clear();
    }
    void conflicts(CompressedData & other, MemAccessMode this_mode, MemAccessMode other_mode,vector<InstrDependence> & out_deps, int64_t & conflict_byte_count){
        out_deps.clear();
        conflict_byte_count = 0;
        for(table_iterator it = other.mem_addr_table.begin(); it != other.mem_addr_table.end(); ++it){
            int64_t mem_addr = it->first;
            PointList & other_pc_data = it->second;
            if(this->mem_addr_table.count(mem_addr)){
                PointList & this_pc_data = this->mem_addr_table[mem_addr];
                conflict_byte_count += this_pc_data.get_access_size();
                vector<PC_Pair> conflict_pairs = this_pc_data.enumerate_conflicts(other_pc_data);
                for(size_t i = 0; i < conflict_pairs.size(); i++){
                    InstrDependence dep(PC_ID(conflict_pairs[i].first,this_mode),PC_ID(conflict_pairs[i].second,other_mode),mem_addr,this_pc_data.get_access_size());
                    out_deps.push_back(dep);
                }
            }
        }
    }
};
