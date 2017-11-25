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
};

class IntersectFinder{
    vector<CompressedSet> data;
    vector<KeyType> keys;
    map<KeyType,size_t> key_locations;
    void add_new_set(KeyType key){
        if(key_locations.count(key)){
            return;
        }
        swap_node_key();
        
        //adds another key entry
        data.push_back(CompressedSet());
        keys.push_back(key);
    }
    void add_values_to_key(KeyType key,const CompressedSet & add_values){
        add_values_to_loc(key_locations[key],add_values);
    }
    bool equal_keys(const IntersectFinder & other){
        if(keys.size() != other.keys.size()){
            return false;
        }
        return equal(keys.begin(),keys.end(), other.keys.begin());
    }

    void merge(const IntersectFinder & other){
        if(equal_keys(other)){
            fast_merge(other);
        }
        else{
            slow_merge(other);
        }
    }
    
    CompressedSet & my_set(KeyType key){
        return data[key_locations[key]];
    }

    size_t num_sets(){
        return (data.size() + 1) / 2;
    }
    vector<KeyType> find_overlap_keys(const CompressedSet & with){
        vector<KeyType> res;
        add_overlap_keys(res,with,0);
        return res;
    }
    vector<IntersectInfo>  conflicting_keys(const IntersectFinder & other){
        vector<IntersectInfo> res;
        vector<KeyType> my_conflict_keys = find_overlap_keys(other.data[0]);
        for(size_t i = 0; i < my_conflict_keys.size(); i++){
            KeyType this_key = my_conflict_keys[i];
            vector<KeyType> other_conflict_keys = other.find_overlap_keys(this->data[key_locations[this_key]]);
            for(size_t j = 0; j < other_conflict_keys.size(); j++){
                KeyType other_key = other_conflict_keys[j];
                CompressedSet intersect_set = this->my_set(this_key);
                intersect_set.intersect(other.my_set(other_key));
                int64_t intersect_size = intersect_set.count();
                IntersectInfo intersect = {this_key,other_key,intersect_size};
                res.push_back(intersect);
            }
        }
        return res;
    }
protected:
    void add_overlap_keys(vector<KeyType> & out_keys,const CompressedSet & with,size_t cur_node){
        if(with.has_any_in_intersect(data[cur_node])){
            if(is_data_node(cur_node)){
                out_keys.push_back(keys[cur_node]);
            }
            else{
                add_overlap_keys(out_keys,with,left(cur_node));
                add_overlap_keys(out_keys,with,right(cur_node));
            }
        }
    }
    void add_values_to_loc(size_t loc,const CompressedSet & add_values){
        data[loc].unite(add_values);
        if(!is_root(loc)){
            add_values_to_loc(parent(loc),add_values);
        }
    }
    bool is_data_node(size_t node){
        return node >= num_tmps();
    }

    bool is_root(size_t loc){
        return loc == 0;
    }
    size_t parent(size_t node){
        return (node-1) / 2;
    }
    size_t left(size_t node){
        return node*2 + 1;
    }
    size_t right(size_t node){
        return node*2 + 2;
    }
    size_t num_tmps(){
        return data.size() - num_sets();
    }
    void fast_merge(const IntersectFinder & other){
        //assumes equal_key returns true, fails otherwise.
        assert(data.size() == other.data.size());
        for(size_t i = 0; i < data.size(); i++){
            data[i].unite(other.data[i]);
        }
    }
    void slow_merge(const IntersectFinder & other){
        for(size_t i = other.num_tmps(); i < other.data.size(); i++){
            KeyType add_key = other.keys[i];
            this->add_new_set(add_key);
            this->add_values_to_key(add_key,other.data[i]);
        }
    }

    void swap_node_key(){
        //swaps final key entry with a node entry
        size_t last_set_loc = num_tmps();
        size_t swap_set_loc = data.size();
        KeyType null_key();
        data.push_back(data[last_set_loc]);
        keys.push_back(null_key);
        
        key_locations[keys[last_set_loc]] = swap_set_loc;
        keys[swap_set_loc] = keys[last_set_loc];
        keys[last_set_loc] = null_key;
    }
};
