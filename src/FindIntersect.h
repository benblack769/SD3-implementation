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

bool any_in_intersect(BasicBlockSet & one, BasicBlockSet & other){
    BasicBlockSet inter = one;
    inter &= other;
    return inter.any();
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
    void add_conflict_keys_to(InstrBlockSet & other, vector<IntersectInfo> & inter_info, int64_t approx_mem_addr){
        if(!any_in_intersect(this->union_all(), other.union_all())){
            return;
        }
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
    IntersectFinder();
    void add_block(KeyType key,int64_t start, int64_t size, bool is_read, IntersectFinder & reads, IntersectFinder & writes, CompressedSet & kills){
        int64_t end = start + size;
        int64_t last = end - 1;
        if(block_val(start) == block_val(last)){
            add_actual_block(key,start,size,is_read,reads,writes,kills);
        }
        else{
            int64_t block_split_end = block_val_loc(block_val(last));
            int64_t first_size = block_split_end - start;
            int64_t last_size = end - block_split_end;
            assert(first_size + last_size == size && start + last_size == end);
            add_actual_block(key,start,first_size,is_read,reads,writes,kills);
            add_actual_block(key,block_split_end,last_size,is_read,reads,writes,kills);
        }
    }
    void clear(){
        instr_blocks.clear();
    }
    BasicBlockSet & get_block(int64_t block_loc){
        assert(block_loc % BLOCK_SIZE == 0);
        instr_blocks_iter iter = instr_blocks.find(block_loc);
        return (iter != instr_blocks.end()) ? iter->second.union_all() : empty_bb_set;
    }

    void merge(IntersectFinder & other){
        instr_blocks_iter iter = this->instr_blocks.begin(), outer_iter = other.instr_blocks.begin();
        for(;iter != this->instr_blocks.end() && outer_iter != other.instr_blocks.end(); ){
            int64_t this_key = iter->first;
            int64_t outer_key = outer_iter->first;
            if(this_key == outer_key){
                iter->second.merge_into(outer_iter->second);
                ++iter;
                ++outer_iter;
            }
            else if(this_key < outer_key){
                iter = this->instr_blocks.lower_bound(outer_key);
            }
            else{
                instr_blocks_iter new_outer_iter = outer_iter;
                ++new_outer_iter;
                this->instr_blocks.insert(iter,*new_outer_iter);
                outer_iter = new_outer_iter;
            }
        }
        if(outer_iter != other.instr_blocks.end()){
            this->instr_blocks.insert(outer_iter,other.instr_blocks.end());
        }
    }

    vector<IntersectInfo> conflicting_keys(IntersectFinder & other){
        vector<IntersectInfo> res;
        for(instr_blocks_iter it = other.instr_blocks.begin(); it != other.instr_blocks.end(); ++it){
            this->instr_blocks[it->first].add_conflict_keys_to(it->second,res,it->first);
        }
        return res;
    }
    void subtract_from_all(CompressedSet & outer){
        typename CompressedSet::set_iterator outer_iter = outer.data.begin();
        instr_blocks_iter iter = this->instr_blocks.begin();
        for(;iter != this->instr_blocks.end() && outer_iter != outer.data.end(); ){
            int64_t this_key = iter->first;
            int64_t outer_key = outer_iter->first;
            if(this_key == outer_key){
                iter->second.subtract_from_all(outer_iter->second);
                ++iter;
                ++outer_iter;
            }
            else if(this_key < outer_key){
                iter = this->instr_blocks.lower_bound(outer_key);
            }
            else{
                outer_iter = outer.data.lower_bound(this_key);
            }
        }
    }
protected:

    void add_actual_block(KeyType key,int64_t start, int64_t size, bool is_read, IntersectFinder & reads, IntersectFinder & writes, CompressedSet & kills){
        BasicBlockSet new_b;
        int64_t adj_el = block_loc(start);
        for(int64_t i = adj_el; i < adj_el + size; i++){
            new_b.add(i);
        }
        new_b &= kills.get_block(adj_el);
        if(!is_read){
            BasicBlockSet new_kills = new_b;
            new_kills &= reads.get_block(adj_el);
            kills.unite_block(adj_el,new_kills);
        }
    }
};

