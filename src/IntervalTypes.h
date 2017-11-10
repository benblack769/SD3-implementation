#pragma once
#include <cassert>
#include <iostream>
#include "Types.h"

/*
The Block and SparseStride types here are both considered 
"intervals". An "interval" is a class with the following 
methods:
    
    first(): the lowest location
    last(): the highest location
*/

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

std::ostream &operator<<(std::ostream &os, const SparseStride &obj);
