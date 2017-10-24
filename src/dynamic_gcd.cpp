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

int64_t num_overlap_locations(SparseStride stride, Block block) {
    return stride.locations_after_access(block.begin()) - stride.locations_after_access(block.end());
}

overlap_locs num_overlap_locations(SparseStride one, SparseStride other) {
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
        return overlap_locs(true,0);
    }
    else if (one.stride() == other.stride()) {
        if (delta % one.stride() == 0) {
            return overlap_locs(true,overlap_size / one.stride());
        } else {
            return overlap_locs(true,0);
        }
    } 
    else if (one.stride() % other.stride() == 0 || other.stride() % one.stride() == 0) {

    }
    else if(one.block_size() == other.block_size()){
        if(gcd_stride % delta != 0){
            return overlap_locs(true,0);
        }
        else{
            // TODO: this currently does not implement gcd test corretly. gives a
            // worst case upper bound analysis
            // we can get away with bad solutions here because there aren't many
            // real world cases where we have
            // overlapping strides of differnet lengths that do not divide each othe
            int64_t overlap1 = num_overlap_locations(one,Block(other.first(),other.end()));
            int64_t overlap2 = num_overlap_locations(other,Block(one.first(),one.end()));
            int64_t cap_overlap = min(overlap1,overlap2);            
            return overlap_locs(false,-1);
        }
    }
    else{
        // this case is really bad, so just return that you don't exactly know what to do.
        int64_t overlap1 = num_overlap_locations(one,Block(other.first(),other.end()));
        int64_t overlap2 = num_overlap_locations(other,Block(one.first(),one.end()));
        int64_t cap_overlap = min(overlap1,overlap2);
        return overlap_locs(false,cap_overlap);
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
