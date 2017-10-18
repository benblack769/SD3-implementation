#pragma once
#include "Types.h"
#include <algorithm>
#include <ostream>
using namespace std;

struct Block{
    int64_t begin;
    int64_t end;//past end
    int64_t accesses;
    int64_t length(){
        return end - begin;
    }
};

class SparseStride{
protected:
    int64_t _first;
    int64_t _size;
    int64_t _block_size;
    int64_t _stride;
public:
    SparseStride(int64_t in_first,int64_t in_size,int64_t in_stride,int64_t in_block_size):
        _first(in_first),
        _size(in_size),
        _stride(in_stride),
        _block_size(in_block_size)
    {}

    int64_t last()const{ return _first + (_size-1)*_stride; }
    int64_t first()const {return _first; }
    int64_t size()const {return _size; }
    int64_t block_size()const {return _block_size; }
    int64_t stride()const {return _stride; }
    int64_t end()const{ return _first + _size*_stride; }

    bool is_in_stride(int item);
    void print_in_stride_from_zero();
};
std::ostream& operator<<(std::ostream& os, const SparseStride& obj);
    
int64_t gcd(int64_t a, int64_t b);

int64_t num_overlap(SparseStride one, SparseStride other);
int64_t num_conflict_b(Block one, Block other);
inline int64_t ceil_div(int64_t num,int64_t denom){
    return (num + denom - 1) / denom;
}
