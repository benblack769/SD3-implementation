#pragma once
#include "Types.h"
#include <algorithm>
#include <iostream>
#include <ostream>
#include <cassert>
using namespace std;

class Block {
  protected:
    int64_t _begin;
    int64_t _end; // past end
  public:
    Block(int64_t begin, int64_t end) : _begin(begin), _end(end) {}
    Block():_begin(0), _end(0){}

    int64_t length() const { return _end - _begin; }
    int64_t first() const { return _begin; }
    int64_t begin() const { return _begin; }
    int64_t end() const { return _end; }
    int64_t last() const { return _end-1; }

    bool is_in(int64_t x){ return x >= begin() && x < end(); }
    void print_in_stride_from_zero();
    bool operator == (Block other)const{
        return this->begin() == other.begin() && this->end() == other.end();
    }
};
std::ostream &operator<<(std::ostream &os, const Block &obj);

class SparseStride {
  protected:
    int64_t _first;
    int64_t _size;
    int64_t _block_size;
    int64_t _stride;

  public:
    SparseStride(int64_t in_first, int64_t in_size, int64_t in_stride, int64_t in_block_size)
    {
        assert(in_stride != 0 && "strides should always be non-zero lenght");
        //first is lowest access. To ensure this, flip around stride when negative strides are input
        if(in_stride < 0){
            in_first += in_size * in_stride;
            in_stride = -in_stride;
        }
        _first = in_first;
        _size = in_size;
        _block_size = in_block_size;
        _stride = in_stride;
    }
    SparseStride()
        : _first(0), _size(0), _stride(0), _block_size(0){}

    int64_t last() const { return _first + (_size - 1) * _stride; }
    int64_t first() const { return _first; }
    int64_t size() const { return _size; }
    int64_t block_size() const { return _block_size; }
    int64_t stride() const { return _stride; }
    int64_t end() const { return _first + _size * _stride; }
    bool is_dense()const{ return _stride == _block_size; }
    Block dense_block()const{ return Block(first(),end()); }
    
    bool is_in(int64_t access);
    Block block(int64_t access);
    int64_t locations_after_access(int64_t access);
};

template<class IntervalType>
inline void print_is_in_from_zero(IntervalType interval) {
    cout << interval << endl;
    for (int i = 0; i < interval.end(); i++) {
        if (interval.is_in(i)) {
            cout << "X";
        } else {
            cout << "O";
        }
    }
    cout << endl;
}
std::ostream &operator<<(std::ostream &os, const SparseStride &obj);

int64_t gcd(int64_t a, int64_t b);

//overlap_locs num_overlap_locations(SparseStride one, SparseStride other);
int64_t num_overlap_locations(SparseStride stride, Block block);
int64_t num_overlap_locations(Block one, Block other);

inline bool inclusive_between(int64_t middle, int64_t low, int64_t high){
    return low <= middle && middle <= high;
}

bool has_overlap(SparseStride one, SparseStride other);
bool has_overlap(SparseStride stride, Block block);
bool has_overlap(Block one, Block other);

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
            abs(one.first() - other.first()) % one.stride() == 0 &&
            (inclusive_between(one.first(),other.first() - other.stride(),other.end()) ||
             inclusive_between(one.last(),other.first() - other.stride(),other.end()));
}

SparseStride merge(SparseStride one,SparseStride other);
