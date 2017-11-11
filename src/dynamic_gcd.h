#pragma once
#include "Types.h"
#include "IntervalTypes.h"
using namespace std;

int64_t gcd(int64_t a, int64_t b);

int64_t num_overlap_locations(SparseStride stride, Block block);
int64_t num_overlap_locations(Block one, Block other);

inline bool inclusive_between(int64_t middle, int64_t low, int64_t high){
    return low <= middle && middle <= high;
}

bool has_overlap(SparseStride one, SparseStride other);
bool has_overlap(SparseStride stride, Block block);
bool has_overlap(Block one, Block other);
inline bool has_overlap(Block block,SparseStride stride){ return has_overlap(stride,block); }

bool contains(Block block, SparseStride stride);
bool contains(SparseStride stride, Block block);
bool contains(Block b1, Block b2);
bool contains(SparseStride s1, SparseStride s2);

inline int64_t ceil_div(int64_t num, int64_t denom) { return (num + denom - 1) / denom; }
inline int64_t locations_to_accesses(int64_t location_overlap, int64_t total_locations, int64_t total_acccesses) {
    return ceil_div(total_acccesses * location_overlap, total_locations);
}

inline bool mergeable(SparseStride one,SparseStride other){
    return one.block_size() == other.block_size() &&
            one.stride() == other.stride() &&
            //difference allows merging
            abs(one.first() - other.first()) % one.stride() == 0 &&
            //first or last element of 1 can be appended to one end of other
            (inclusive_between(one.first(),other.first() - other.stride(),other.end()) ||
             inclusive_between(one.last(),other.first() - other.stride(),other.end()));
}

SparseStride merge(SparseStride one,SparseStride other);
