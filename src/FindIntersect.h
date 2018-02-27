#pragma once
#include "PC_ID.h"
#include "CompressedBits.h"
#include <vector>
#include <map>
#include <cassert>

typedef PC_ID KeyType;
struct IntersectInfo{
    KeyType key_one;
    KeyType key_two;
    int64_t size_of_intersect;
    int64_t approx_mem_addr;
};

extern int64_t add_mem_time;

inline bool any_in_intersect(BasicBlockSet & one, BasicBlockSet & other){
    BasicBlockSet inter = one;
    inter &= other;
    return inter.any();
}

inline int64_t count_in_intersect(BasicBlockSet & one, BasicBlockSet & other){
    BasicBlockSet inter = one;
    inter &= other;
    return inter.count();
}

class InstrBlockSet{
protected:
    BasicBlockSet _union_all;
    map<KeyType, BasicBlockSet> instr_keys;
    typedef typename map<KeyType, BasicBlockSet>::iterator map_iter;
public:
    void add_key_data(const KeyType & key, const BasicBlockSet & set){
        instr_keys[key] |= set;
        _union_all |= set;
    }
    BasicBlockSet & union_all(){
        return _union_all;
    }
    void add_conflict_keys_to(InstrBlockSet & other, vector<IntersectInfo> & inter_info, int64_t approx_mem_addr, int64_t & mem_footprint){
        if(!any_in_intersect(this->union_all(), other.union_all())){
            return;
        }
        mem_footprint += count_in_intersect(this->union_all(), other.union_all());
        for(map_iter t_it = this->instr_keys.begin(); t_it != this->instr_keys.end(); ++t_it){
            KeyType this_key = t_it->first;
            BasicBlockSet this_set = t_it->second;
            for(map_iter o_it = other.instr_keys.begin(); o_it != other.instr_keys.end(); ++o_it){
                KeyType other_key = o_it->first;
                BasicBlockSet other_set = o_it->second;
                other_set &= this_set;
                if(other_set.any()){
                    IntersectInfo info = { this_key, other_key, other_set.count(), approx_mem_addr };
                    inter_info.push_back(info);
                }
            }
        }
    }
    void merge_into(InstrBlockSet & other){
        _union_all |= other._union_all;
        for(map_iter it = other.instr_keys.begin(); it != other.instr_keys.end(); ++it){
            instr_keys[it->first] |= it->second;
        }
    }
    void subtract_from_all(BasicBlockSet & sub){
        _union_all.subtract(sub);
        for(map_iter it = instr_keys.begin(); it != instr_keys.end();){
            it->second.subtract(sub);
            if(!it->second.any()){
                instr_keys.erase(it++);
            }
            else{
                ++it;
            }
        }
    }
};

class IntersectFinder{
protected:
    map<int64_t,InstrBlockSet> instr_blocks;
    typedef typename map<int64_t, InstrBlockSet>::iterator instr_blocks_iter;
public:
    IntersectFinder(){}
    void add_block(KeyType key,int64_t start, int64_t size, bool is_read, IntersectFinder & reads, IntersectFinder & writes){
        int64_t end = start + size;
        int64_t last = end - 1;
        if(block_val(start) == block_val(last)){
            add_actual_block(key,start,size,is_read,reads,writes);
        }
        else{
            int64_t block_split_end = block_val_loc(block_val(last));
            int64_t first_size = block_split_end - start;
            int64_t last_size = end - block_split_end;
            assert(first_size + last_size == size && start + last_size == end);
            add_actual_block(key,start,first_size,is_read,reads,writes);
            add_actual_block(key,block_split_end,last_size,is_read,reads,writes);
        }
    }
    void clear(){
        instr_blocks.clear();
    }
    BasicBlockSet & get_block(int64_t block_loc){
        instr_blocks_iter iter = instr_blocks.find(block_loc);
        return (iter != instr_blocks.end()) ? iter->second.union_all() : empty_bb_set;
    }

    void merge(IntersectFinder & other){
        for(instr_blocks_iter it = other.instr_blocks.begin(); it != other.instr_blocks.end(); ++it){
            this->instr_blocks[it->first].merge_into(it->second);
        }
    }

    vector<IntersectInfo> conflicting_keys(IntersectFinder & other,int64_t & total_mem_footprint){
        vector<IntersectInfo> res;
        for(instr_blocks_iter it = other.instr_blocks.begin(); it != other.instr_blocks.end(); ++it){
            if(this->instr_blocks.count(it->first)){
                this->instr_blocks[it->first].add_conflict_keys_to(it->second,res,it->first,total_mem_footprint);
            }
        }
        return res;
    }
    void subtract_from_all(IntersectFinder & reads, IntersectFinder & writes){
        for(instr_blocks_iter it = this->instr_blocks.begin(); it != this->instr_blocks.end(); ++it){
            BasicBlockSet remove = writes.get_block(it->first);
            remove.subtract(reads.get_block(it->first));
            if(remove.any()){
                it->second.subtract_from_all(remove);
            }
        }
    }
protected:
    void add_actual_block(KeyType key,int64_t start, int64_t size, bool is_read, IntersectFinder & reads, IntersectFinder & writes){
        BasicBlockSet new_b;
        int64_t block_num = block_val(start);
        int64_t adj_el = block_loc(start);
        for(int64_t i = adj_el; i < adj_el + size; i++){
            new_b.add(i);
        }
        BasicBlockSet kills = writes.get_block(block_num);
        kills.subtract(reads.get_block(block_num));
        new_b.subtract(kills);
        if(new_b.any()){
            instr_blocks[block_num].add_key_data(key,new_b);
        }
    }
};

