#include "dynamic_gcd.h"
#include <iostream>
#include <cassert>

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
    os << " { 'First':" << obj.first() << ", 'Last':" << obj.last() << ", 'Size':" << obj.size() <<
          ", 'Stride':" << obj.stride() << ", 'BlockSize':" << obj.block_size() << " } ";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Block &obj){
    os << " { 'begin':" << obj.begin() << ", 'end':" << obj.end() << " } ";
    return os;
}
