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
StrideIterator begin(SparseStride stride){
    StrideIterator res = {stride,0};
    return res;
}

bool slow_has_overlap (StrideIterator one, StrideIterator other){
  while (!one.at_end() && !other.at_end())
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
bool slow_has_overlap(SparseStride one, SparseStride other){
    return slow_has_overlap(begin(one),begin(other));
}

struct ExtEuclidResult{
    int64_t gcd_result;
    int64_t a_value;
    int64_t b_value;
};

// C function for extended Euclidean Algorithm
//Taken from https://en.wikibooks.org/wiki/Algorithm_Implementation/Mathematics/Extended_Euclidean_algorithm
/*ExtEuclidResult xgcd(int64_t a, int64_t b){
    int64_t aa[2]={1,0}, bb[2]={0,1}, q;
    while(true) {
        q = a / b; a = a % b;
        aa[0] = aa[0] - q*aa[1];  bb[0] = bb[0] - q*bb[1];
        if (a == 0) {
            ExtEuclidResult result = {b,aa[1],bb[1]};
            return result;
        };
        q = b / a; b = b % a;
        aa[1] = aa[1] - q*aa[0];  bb[1] = bb[1] - q*bb[0];
        if (b == 0) {
            ExtEuclidResult result = {a,aa[0],bb[0]};
            return result;
        };
    };
}*/
bool i_divides(int64_t denom,int64_t num){
    return num % denom == 0;
}
bool has_overlap(SparseStride one, SparseStride other) {
    // ensures one.first < other.first always
    if (one.first() > other.first()) {
        SparseStride tmp = one;
        one = other;
        other = tmp;
    }
    int64_t delta = other.first() - one.first();
    int64_t low = one.first();
    int64_t high = min(one.last(),other.last());
    int64_t overlap_size = high - low;

    if (overlap_size <= 0) {
        return false;
    }
    else if(delta == 0){
        return true;
    }
    else if(one.is_dense() || other.is_dense()){
        //TODO: this case is not always true! Consider a really short stride inside one with a huge stride length.
        return true;
    }
    else if(one.block_size() == other.block_size() && 
            i_divides(one.block_size(),delta)){
        if(one.stride() == other.stride()){
            return i_divides(one.stride(),delta);
        }
        //ExtEuclidResult res = xgcd(one.stride(),other.stride());        
        int64_t gcd = ::gcd(one.stride(),other.stride());//res.gcd_result;
        if(i_divides(gcd,delta)){
            /*int64_t lcm = (one.stride() * other.stride()) / gcd;
            int64_t adj_b_val = (res.b_value * delta) / gcd;
            int64_t first_conflict = adj_b_val * other.stride() + other.first();
            while(first_conflict < other.first()){
                first_conflict += lcm;
            }
            return first_conflict <= one.last() && 
                   first_conflict <= other.last();*/
            int64_t lcm = (one.stride() * other.stride()) / gcd;
            if(lcm <= overlap_size){
                return true;
            }
            else{
                return slow_has_overlap(one,other);
            }
        }
        else{
            return false;
        }
    }
    else{
        return slow_has_overlap(one,other);
    }
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
