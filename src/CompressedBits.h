#pragma once
#include <map>
#include <bitset>
#include <cassert>
#include <ostream>
#include "Types.h"

using namespace std;

//make sure this is a power of 2
#define BLOCK_SIZE 256

//enum BlockType{BitsetBlock,VecBlock};

typedef unsigned char uint8_t;

class BasicBlockSet{
    /*
    use:
        helper class for CompressedBits. Not particularly useful otherwise

    implementation:
        Simply wraps over bitset for now. There are possible optimizations, though,
        so this provides a generic interface for those optimiziations.
    */
protected:
    bitset<BLOCK_SIZE> bits;
public:
    BasicBlockSet(){}
    void add(int64_t element){ assert(element < BLOCK_SIZE && element >= 0); bits[element] = 1; }
    bool has(int64_t element){ assert(element < BLOCK_SIZE && element >= 0); return bits[element]; }
    void operator |= (const BasicBlockSet & outer) { bits |= outer.bits; }
    void operator &= (const BasicBlockSet & outer) { bits &= outer.bits; }
    void subtract(const BasicBlockSet &  outer){ bits &= ~outer.bits; }
    bool any(){ return bits.any(); }
    int64_t count(){ return bits.count(); }
    //bool contains(const BlockSet &  outer);
    friend class CompressedSet;
    friend ostream & operator << (ostream & os,const BasicBlockSet & bitset);
};
inline int64_t block_val(int64_t loc){
    return loc / BLOCK_SIZE;
}
inline int64_t block_val_loc(int64_t val){
    return val * BLOCK_SIZE;
}
inline int64_t block_loc(int64_t loc){
    return loc % BLOCK_SIZE;
}
extern BasicBlockSet empty_bb_set;
