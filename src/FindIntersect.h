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

extern int64_t add_mem_time;


class IntersectFinder{
protected:
    vector<CompressedSet> data;
    vector<KeyType> keys;
    map<KeyType,size_t> key_locations;
    
    bool needs_update;
    vector<char> update_keys;
public:
    IntersectFinder();
    void add_new_set(KeyType key);
    void add_values_to_key(KeyType key,CompressedSet & add_values);
    bool is_empty(){
        return data.size() == 0;
    }

    void clear();

    void merge(IntersectFinder & other);
    void subtract_from_all(CompressedSet & add_values);

    CompressedSet & my_set(KeyType key);
    CompressedSet & union_all();

    size_t num_keys(){
        return key_locations.size();
    }

    vector<IntersectInfo> conflicting_keys(IntersectFinder & other);
protected:
    vector<KeyType> find_overlap_keys(CompressedSet & with);
    bool equal_keys(IntersectFinder & other);
    void add_overlap_keys(vector<KeyType> & out_keys,CompressedSet & with,size_t cur_node);
    void subtract_from(CompressedSet & with,size_t cur_node);
    void add_values_to_loc(size_t loc,CompressedSet & add_values);
    bool is_data_node(size_t node){
        return node >= num_tmps();
    }
    void update_intermeds();

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
        return data.size() - num_keys();
    }
    void slow_merge(IntersectFinder & other);

    void swap_node_key();
};

