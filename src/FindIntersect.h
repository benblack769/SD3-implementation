#pragma once
#include "PC_ID.h"
#include "CompressedBits.h"
#include <vector>
#include <map>
#include <list>
#include <cassert>

typedef PC_ID KeyType;
struct IntersectInfo{
    KeyType key_one;
    KeyType key_two;
    int64_t size_of_intersect;
};

extern int64_t add_mem_time;

class CompressedIntersectFinder;
class FastIntersectFinder;

class CompressedIntersectFinder{
protected:
    vector<CompressedSet> data;
    vector<KeyType> keys;
    map<KeyType,size_t> key_locations;
    
    bool needs_update;
    vector<char> update_keys;
public:
    void add_new_set(KeyType key);
    void add_values_to_key(KeyType key,CompressedSet & add_values);
    bool is_empty(){
        return data.size() == 0;
    }

    void clear();

    void merge(CompressedIntersectFinder & other);
    void subtract_from_all(CompressedSet & add_values);

    CompressedSet & my_set(KeyType key);
    CompressedSet & union_all();


    vector<IntersectInfo> conflicting_keys(CompressedIntersectFinder & other);
protected:
    vector<KeyType> find_overlap_keys(CompressedSet & with);
    bool equal_keys(CompressedIntersectFinder & other);
    void add_overlap_keys(vector<KeyType> & out_keys,CompressedSet & with,size_t cur_node);
    void subtract_from(CompressedSet & with,size_t cur_node);
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
    
    size_t num_keys(){
        return key_locations.size();
    }
    
    void slow_merge(CompressedIntersectFinder & other);

    void swap_node_key();
};

struct AccInfo{
    int64_t size;
    KeyType key;
};
void extend_vec(list<AccInfo> & extended,  list<AccInfo> & add){
    extended.insert(extended.end(),add.begin(),add.end());
}

class FastIntersectFinder{
protected:
    map<int64_t,list<AccInfo> > accesses;
    typedef typename unordered_map<int64_t,list<AccInfo> >::iterator acc_iterator;
    typedef typename list<AccInfo>::iterator list_iterator;
public:
    void add_new_element(KeyType key,int64_t location,int64_t size){
        AccInfo new_info = {size,key};
        accesses[location].push_back(new_info);
    }
    void clear(){
        accesses.clear();
    }
    void merge(FastIntersectFinder & other){
        for(acc_iterator it = other.accesses.begin(); it != other.accesses.end(); ++it){
            extend_vec(accesses[it->first],it->second);
        }
    }

    vector<IntersectInfo> conflicting_keys(FastIntersectFinder & other){
        vector<IntersectInfo> res;
        for(acc_iterator iter = this->accesses.begin(), outer_iter = other.accesses.begin();
                iter != this->accesses.end() && outer_iter != other.accesses.end(); ){
            int64_t this_key = iter->first;
            int64_t outer_key = outer_iter->first;
            if(this_key == outer_key){
                list<AccInfo> & this_list = iter->second;
                list<AccInfo> & other_list = outer_iter->second;
                for(list_iterator t_it = this_list.begin(); t_it != this_list.end(); ++t_it){
                    for(list_iterator o_it = other_list.begin(); o_it != other_list.end(); ++o_it){
                        IntersectInfo new_info = {t_it->key,o_it->key,t_it->size};
                        res.push_back(new_info);
                    }
                }
            }
            else if(this_key < outer_key){
                iter = this->accesses.lower_bound(outer_key);
            }
            else{
                outer_iter = other.accesses.lower_bound(this_key);
            }
        }
    }
    
    CompressedIntersectFinder compress(){
        
    }
};

class IntersectFinder{
public:
    FastIntersectFinder fast_finder;
    CompressedIntersectFinder inter_finder;
    CompressedSet _union_all;
    bool is_fast;
    
    void add_new_element(KeyType key,int64_t location,int64_t size){
        
    }
    
    void clear(){
        if(is_fast){
            fast_finder.clear();
        }
        else{
            inter_finder.clear();
        }
    }

    CompressedSet & union_all(){return _union_all;}

    void merge(IntersectFinder & other){
        if(is_fast && other.is_fast){
            fast_finder.merge(other.fast_finder);
        }
        else{
            convert();
            other.convert();
            inter_finder.merge(other.inter_finder);
        }
    }

    void subtract_from_all(CompressedSet & add_values){
        convert();
        inter_finder.subtract_from_all(add_values);
    }

    vector<IntersectInfo> conflicting_keys(IntersectFinder & other){
        if(is_fast && other.is_fast){
            return fast_finder.conflicting_keys(other.fast_finder);
        }
        else{
            convert();
            other.convert();
            return inter_finder.conflicting_keys(other.inter_finder);
        }
    }

protected:
    void convert(){
        if(is_fast){
            is_fast = false;
            inter_finder = fast_finder.compress();
            fast_finder.clear();
        }
    }
};
