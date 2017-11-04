#include "dynamic_gcd.h"
#include <iostream>
#include <cassert>

int64_t gcd(int64_t a, int64_t b) {
    int64_t max_v = max(a, b);
    int64_t min_v = min(a, b);
    while (min_v != 0) {
        int64_t new_min = max_v % min_v;
        max_v = min_v;
        min_v = new_min;
    }
    return max_v;
}
int64_t num_overlap_locations(Block one, Block other) {
    int64_t overlap_between1 = other.end() - one.begin();
    int64_t overlap_between2 = one.end() - other.begin();
    int64_t overlap_inner_1 = one.end() - one.begin();
    int64_t overlap_inner_2 = other.end() - other.begin();
    int64_t inner_min = min(overlap_inner_1,overlap_inner_2);
    int64_t between_min = min(overlap_between1,overlap_between2);
    if(between_min > inner_min){
        return inner_min;
    }
    else if(between_min > 0){
        return between_min;
    }
    else{
        return 0;
    }
}
int64_t SparseStride::locations_after_access(int64_t access){
    int64_t num_locs = 0;
    //counts access in same block as `access`
    if(is_in(access)){
        Block b = block(access);
        num_locs += b.end() - access;
    }
    //counts accesses after `access`
    int64_t after = end() - access;
    int64_t num = (after-1) / stride();
    num_locs += num * block_size();
    return num_locs;
}

bool contains(Block block, SparseStride stride){
    return contains(block,stride.dense_block());
}
bool contains(SparseStride stride, Block block){
    if(!stride.is_in(block.begin())){
        return false;
    }
    return contains(stride.block(block.begin()),block);
}

bool contains(Block b1, Block b2){
    return b1.begin() >= b2.begin() && b1.end() <= b2.end();
}

bool contains(SparseStride s1, SparseStride s2){
    if(s1.is_dense()){
        return contains(s1.dense_block(), s2);
    }
    else{
        return contains(s1.dense_block(),s2.dense_block()) && 
                s2.stride() % s1.stride() == 0 && 
                s1.block_size() == s2.block_size(); 
    }
}

int64_t num_overlap_locations(SparseStride stride, Block block) {
    return stride.locations_after_access(block.begin()) - stride.locations_after_access(block.end());
}

bool has_overlap(SparseStride stride, Block block){
    return num_overlap_locations(stride,block) > 0;
}

bool has_overlap(Block one, Block other){
    return num_overlap_locations(one,other) > 0;
}

struct StrideIterator{
    SparseStride stride;
    int64_t cur_step;
    Block operator * (){
        int64_t begin = stride.first()+stride.stride()*cur_step;
        return Block(begin,begin+stride.block_size());
    }
    void operator ++(){
        cur_step++;
    }
    bool at_end(){
        return cur_step >= stride.size();
    }
    
};

bool slow_has_overlap (StrideIterator one, StrideIterator other){
  while (one.at_end() && other.at_end())
  {
      if(has_overlap(*one,*other)){
          return true;
      }
      else if((*one).first() < (*other).first()) ++one;
      else if((*other).first() < (*one).first()) ++other;
      else{
          assert(false && "condition should not occcur");
      }
  }
  return false;
}



bool has_overlap(SparseStride one, SparseStride other) {
    // ensures one.first < other.first always
    if (one.first() > other.first()) {
        SparseStride tmp = one;
        one = other;
        other = tmp;
    }
    int64_t delta = other.first() - one.first();
    int64_t overlap_size = one.last() - other.first();
    int64_t gcd_stride = gcd(one.stride(),other.stride());

    if (overlap_size < 0) {
        return false;
    }
    else if (one.stride() == other.stride()) {
        if (delta % one.stride() == 0) {
            return true;
        } 
    } 
    else if (one.stride() % other.stride() == 0 || other.stride() % one.stride() == 0) {
        
    }
    else if(one.block_size() == other.block_size()){
        if(gcd_stride % delta != 0){
            return true;
        }
        else{
            //slow_has_overlap()
            assert(false);
            // TODO: this currently does not implement gcd test corretly. gives a
            // worst case upper bound analysis
            // we can get away with bad solutions here because there aren't many
            // real world cases where we have
            // overlapping strides of differnet lengths that do not divide each othe
            int64_t overlap1 = num_overlap_locations(one,Block(other.first(),other.end()));
            int64_t overlap2 = num_overlap_locations(other,Block(one.first(),one.end()));
            int64_t cap_overlap = min(overlap1,overlap2);            
            return false;//overlap_locs(false,-1);
        }
    }
    else{
        assert(false);
        // this case is really bad, so just return that you don't exactly know what to do.
        int64_t overlap1 = num_overlap_locations(one,Block(other.first(),other.end()));
        int64_t overlap2 = num_overlap_locations(other,Block(one.first(),one.end()));
        int64_t cap_overlap = min(overlap1,overlap2);
        return false;//overlap_locs(false,cap_overlap);
    }
}
bool SparseStride::is_in(int64_t access) {
    int64_t begin_offset = access - first();
    if (begin_offset < 0) {
        return false;
    }
    int64_t stride_offset = begin_offset % stride();
    return stride_offset < block_size();
}

Block SparseStride::block(int64_t access) {
    assert(is_in(access) && "block method requires access to be in some block");
    
    int64_t begin_offset = access - first();
    int64_t block_number = begin_offset / stride();
    int64_t block_start = first() + block_number * stride();
    return Block(block_start, block_start + block_size());
}

std::ostream &operator<<(std::ostream &os, const SparseStride &obj) {
    os << " { 'F':" << obj.first() << ", 'L':" << obj.last() << ", 'S':" << obj.stride()
       << ", 'BS':" << obj.block_size() << " } ";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Block &obj){
    os << " { 'begin':" << obj.begin() << ", 'end':" << obj.end() << " } ";
    return os;
}

SparseStride ordered_merge(SparseStride lower_eq, SparseStride upper){
    //strides and block_size are enforced equal
    int64_t first = lower_eq.first();
    int64_t stride = upper.stride();
    int64_t end = max(lower_eq.end(),upper.end());
    assert((end - first) % stride == 0);
    int64_t total_size = (end - first) / stride;
    return SparseStride(first,total_size,stride,upper.block_size());    
}

SparseStride merge(SparseStride one,SparseStride other){
    assert(mergeable(one,other));
    return one.first() < other.first() ? ordered_merge(one,other) : ordered_merge(other,one);
}
