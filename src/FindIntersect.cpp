#include "FindIntersect.h"

/*
The idea is this:

For compressed memory access, we want 3 things

2 sets of instructions, each with a set of memory locations they accessed

You want to be able to

1. Merge these together
2. Find instructions that have overlapping memory addresses


There are other operations involving a single set such as

1. Remove all bits that are in a set of adresses
2. Find the union of all memory adresses

In order to accomplish all these things efficiently, we be constantly relying on the
distributive property of set intersection over union.

When thinking about sets this way, you can reaslize that you can do binary
search over sets. The only thing you need to do this is the union of each pair of sets.

The idea is to store the instructions in a tree. The leaves are the instructions,
the nodes are unions of the sets in the children.

The root, of course, is the union of all the sets in the tree.

For example,
              {1,2,3,4}
        {1,3,4}          {1,2}
  {1,4}I:4  {3}I:2  {1,2}I:1  {2}I:5

There is also a bidirectional map between instructions and locations in the tree,
so that data access is efficient.


The operations on this set is as follows:

1. Adding new instruction to the set is just like adding elements to a heap. This results in perfect ballancing.
2. Merging in new memory accesses for an existing instruction finds the instruction, and unions the set with all of the parents of that node
3. Finding conflicts between two trees happens in two steps
    1. Pick the root of one of the trees. Recursively go down the other tree into the nodes which have overlap with the root. Save the instructions with those nodes
    2. For each of those instructions, recursively go down the original tree to

Running time of operations looks like this:

Assume linear time for set intersection and union, possible with hash tables.

N is the total number of instructions
M is the total number of memory adresses accesse

1. This is constant amortized time
2. This is O(m * log(N)), where m is the number of memory adfresses that you are unioning in
3.
    1. This O(M * log(N) * C) Where C is the number of pairs of instruction conflicts
*/

void IntersectFinder::add_new_set(KeyType key){
    if(key_locations.count(key)){
        return;
    }
    if(!is_empty()){
        swap_node_key();
    }
    
    //adds another key entry
    data.push_back(CompressedSet());
    keys.push_back(key);
    key_locations[key] = keys.size()-1;
}
void IntersectFinder::add_values_to_key(KeyType key,CompressedSet & add_values){
    size_t key_loc = key_locations[key];
    data[key_loc].unite(add_values);
    data[0].unite(add_values);
}
void IntersectFinder::create_intermeds(){
    const int64_t final_non_all_parent_num = 3;
    for(int64_t i = int64_t(data.size())-1; i >= final_non_all_parent_num; i--){
        data[parent(i)].unite(data[i]);
    }
}
void IntersectFinder::delete_intermeds(){
    for(size_t i = 1; i < num_tmps(); i++){
        data[i].clear();
    }
}

void IntersectFinder::clear(){
    data.clear();
    keys.clear();
    key_locations.clear();
}

void IntersectFinder::merge(IntersectFinder & other){
    for(size_t i = other.num_tmps(); i < other.data.size(); i++){
        KeyType add_key = other.keys[i];
        this->add_new_set(add_key);
        this->add_values_to_key(add_key,other.data[i]);
    }
}
void IntersectFinder::subtract_from_all(CompressedSet & add_values){
    if(!is_empty()){
        data[0].subtract(add_values);

        for(size_t i = num_tmps(); i < data.size(); i++){
            data[i].subtract(add_values);
        }
    }
}

CompressedSet & IntersectFinder::my_set(KeyType key){
    return data[key_locations[key]];
}
CompressedSet & IntersectFinder::union_all(){
    if(is_empty()){
        return empty_set;
    }
    else{
        return data[0];
    }
}

vector<IntersectInfo>  IntersectFinder::conflicting_keys(IntersectFinder & other){
    vector<IntersectInfo> res;
    if(is_empty()){
        return res;
    }
    create_intermeds();
    vector<KeyType> my_conflict_keys = find_overlap_keys(other.union_all());
    for(size_t i = 0; i < my_conflict_keys.size(); i++){
        KeyType this_key = my_conflict_keys[i];
        CompressedSet this_set = this->my_set(this_key);
        
        vector<KeyType> other_conflict_keys = other.find_overlap_keys(this->data[key_locations[this_key]]);
        for(size_t j = 0; j < other_conflict_keys.size(); j++){
            KeyType other_key = other_conflict_keys[j];
            CompressedSet intersect_set = this_set;
            intersect_set.intersect(other.my_set(other_key));
            int64_t intersect_size = intersect_set.count();
            IntersectInfo intersect = {this_key,other_key,intersect_size};
            res.push_back(intersect);
        }
    }
    delete_intermeds();
    return res;
}
vector<KeyType> IntersectFinder::find_overlap_keys(CompressedSet & with){
    vector<KeyType> res;
    add_overlap_keys(res,with,0);
    return res;
}
void IntersectFinder::add_overlap_keys(vector<KeyType> & out_keys,CompressedSet & with,size_t cur_node){
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

void IntersectFinder::swap_node_key(){
    //swaps final key entry with a node entry
    size_t last_set_loc = num_tmps();
    size_t swap_set_loc = data.size();
    assert(parent(swap_set_loc) == last_set_loc && "this is needed to ensure correctness");
    KeyType null_key;
    if(last_set_loc == 0){
        data.push_back(data[last_set_loc]);
    }
    else{
        data.push_back(CompressedSet());
        data[last_set_loc].swap(data.back());
    }
    keys.push_back(null_key);

    key_locations[keys[last_set_loc]] = swap_set_loc;
    keys[swap_set_loc] = keys[last_set_loc];
    keys[last_set_loc] = null_key;
}