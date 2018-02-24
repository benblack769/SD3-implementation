#include "FindIntersect.h"
#include <algorithm>

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

IntersectFinder::IntersectFinder(){
    union_data.resize(1);
}
void IntersectFinder::add_new_set(KeyType key){
    if(key_locations.count(key)){
        return;
    }
    key_data.push_back(CompressedSet());
    keys.push_back(key);
    key_locations[key] = keys.size()-1;
}
void IntersectFinder::add_values_to_key(KeyType key,CompressedSet & add_values){
    size_t key_loc = key_locations[key];
    key_data[key_loc].unite(add_values);
    union_data[0].unite(add_values);
}
void IntersectFinder::resize_unions(){
    size_t new_union_size = keys.size() / 2;
    if(new_union_size > 1 && new_union_size != union_data.size()){
        union_data.resize(new_union_size);
    }
}
struct NodeOrder{
    int64_t overhead;
    size_t old_loc;
    bool operator < (NodeOrder order)const{
        return overhead > order.overhead;
    }
};
void IntersectFinder::sort_key_data(){
    size_t num_keys = keys.size();
    vector<NodeOrder> sorted_order(num_keys);
    for(size_t i = 0; i < num_keys; i++){
        NodeOrder val = {overhead_value(i),i};
        sorted_order[i] = val;
    }
    sort(sorted_order.begin(), sorted_order.end());

    vector<KeyType> new_keys(num_keys);
    vector<CompressedSet> new_key_data(num_keys);
    for(size_t i = 0; i < num_keys; i++){
        size_t old_l = sorted_order[i].old_loc;
        key_locations[keys[old_l]] = i;
        new_keys[i] = keys[old_l];
        new_key_data[i].swap(key_data[old_l]);
    }
    new_keys.swap(keys);
    new_key_data.swap(key_data);
}
void IntersectFinder::update_intermeds(){
    sort_key_data();
    union_data.resize(1);
    resize_unions();
    for(int64_t i = int64_t(key_data.size())-1; i >= 0; i--){//union_data[0] always up to date
        if(data_parent(i) > 0){//union_data[0] always up to date
            union_data[data_parent(i)].unite(key_data[i]);
        }
    }
    for(int64_t i = int64_t(union_data.size())-1; i >= 0; i--){
        if(node_parent(i) > 0){//union_data[0] always up to date
            union_data[node_parent(i)].unite(union_data[i]);
        }
    }
}

void IntersectFinder::clear(){
    union_data.resize(1);
    union_data[0].clear();
    key_data.clear();
    key_locations.clear();
    keys.clear();
}

void IntersectFinder::merge(IntersectFinder & other){
    slow_merge(other);
}
void IntersectFinder::subtract_from_all(CompressedSet & add_values){
    for(size_t i = 0; i < union_data.size(); i++){
        union_data[i].subtract(add_values);
    }
    for(size_t i = 0; i < key_data.size(); i++){
        key_data[i].subtract(add_values);
    }
}

CompressedSet & IntersectFinder::my_set(KeyType key){
    return key_data[key_locations[key]];
}
CompressedSet & IntersectFinder::union_all(){
    //update_intermeds(); // not needed since data[0] is always kept up to date
    return union_data[0];
}

vector<IntersectInfo>  IntersectFinder::conflicting_keys(IntersectFinder & other){
    vector<IntersectInfo> res;
    if(is_empty()){
        return res;
    }
    update_intermeds();
    other.update_intermeds();
    vector<KeyType> my_conflict_keys = find_overlap_keys(other.union_all());
    for(size_t i = 0; i < my_conflict_keys.size(); i++){
        KeyType this_key = my_conflict_keys[i];
        CompressedSet & this_set = this->my_set(this_key);
        
        vector<KeyType> other_conflict_keys = other.find_overlap_keys(this_set);
        for(size_t j = 0; j < other_conflict_keys.size(); j++){
            KeyType other_key = other_conflict_keys[j];
            int64_t intersect_size = this_set.count_intersect(other.my_set(other_key));
            IntersectInfo intersect = {this_key,other_key,intersect_size};
            res.push_back(intersect);
        }
    }
    union_data.resize(1);
    other.union_data.resize(1);
    return res;
}
vector<KeyType> IntersectFinder::find_overlap_keys(CompressedSet & with){
    vector<KeyType> res;
    add_overlap_keys(res,with,0);
    return res;
}
void IntersectFinder::add_overlap_keys(vector<KeyType> & out_keys,CompressedSet & with,size_t cur_node){
    if(has_data(cur_node) && with.has_any_in_intersect(key_data[cur_node])){
        out_keys.push_back(keys[cur_node]);
    }
    if(has_union(cur_node) && with.has_any_in_intersect(union_data[cur_node])){
        add_overlap_keys(out_keys,with,left(cur_node));
        add_overlap_keys(out_keys,with,right(cur_node));
    }
}
void IntersectFinder::slow_merge(IntersectFinder & other){
    for(size_t i = 0; i < other.key_data.size(); i++){
        if(other.key_data[i].any()){
            KeyType add_key = other.keys[i];
            this->add_new_set(add_key);
            this->add_values_to_key(add_key,other.key_data[i]);
        }
    }
}
