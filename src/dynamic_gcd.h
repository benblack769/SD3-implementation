#pragma once
#include "Types.h"
#include <algorithm>
#include <iostream>
#include <ostream>
using namespace std;

struct overlap_locs{
    bool exactly;
    int64_t count;
    overlap_locs(bool in_exactly,int64_t in_count):
        exactly(in_exactly),
        count(in_count){}
};

class Block {
  protected:
    int64_t _begin;
    int64_t _end; // past end
  public:
    Block(int64_t begin, int64_t end) : _begin(begin), _end(end) {}

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
        : _first(in_first), _size(in_size), _stride(in_stride), _block_size(in_block_size) {}

    int64_t last() const { return _first + (_size - 1) * _stride; }
    int64_t first() const { return _first; }
    int64_t size() const { return _size; }
    int64_t block_size() const { return _block_size; }
    int64_t stride() const { return _stride; }
    int64_t end() const { return _first + _size * _stride; }
    
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

overlap_locs num_overlap_locations(SparseStride one, SparseStride other);
int64_t num_overlap_locations(SparseStride stride, Block block);
int64_t num_overlap_locations(Block one, Block other);

inline int64_t ceil_div(int64_t num, int64_t denom) { return (num + denom - 1) / denom; }
inline int64_t locations_to_accesses(int64_t location_overlap, int64_t total_locations, int64_t total_acccesses) {
    return ceil_div(total_acccesses * location_overlap, total_locations);
}
