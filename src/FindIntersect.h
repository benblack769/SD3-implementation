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
    vector<CompressedSet> union_data;
    vector<CompressedSet> key_data;
    vector<KeyType> keys;
    map<KeyType,size_t> key_locations;

public:
    IntersectFinder();
    void add_new_set(KeyType key);
    void add_values_to_key(KeyType key,CompressedSet & add_values);
    bool is_empty(){
        return key_data.size() == 0;
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
    void add_overlap_keys(vector<KeyType> & out_keys,CompressedSet & with,size_t cur_node);
    void subtract_from(CompressedSet & with,size_t cur_node);
    void resize_unions();
    void sort_key_data();
    void update_intermeds();

    CompressedSet & node_at(size_t node){
        return has_union(node) ? union_data[node] : key_data[node];
    }
    int64_t data_parent(size_t node){
        return node < union_data.size() ? int64_t(node) : node_parent(node);
    }
    size_t has_union(size_t data_node){
        return data_node < union_data.size();
    }
    int64_t node_parent(size_t node){
        return (int64_t(node)-1) / 2;
    }
    size_t left(size_t node){
        return node*2 + 1;
    }
    size_t right(size_t node){
        return node*2 + 2;
    }
    int64_t neighbor(int64_t node){
        return node % 2 ? node + 1 : node - 1;
    }
    bool has_neighbor(size_t node){
        return node != 0 && neighbor(node) < union_data.size();
    }
    int64_t overhead_value(size_t node){
        return key_data[node].set_overhead();
    }
    bool has_data(size_t node){
        return node < key_data.size();
    }
    bool has_left(size_t node){
        return left(node) < key_data.size();
    }
    bool has_right(size_t node){
        return right(node) < key_data.size();
    }
    //size_t num_tmps(){
    //    return data.size() - num_keys();
    //}
    void slow_merge(IntersectFinder & other);
};

