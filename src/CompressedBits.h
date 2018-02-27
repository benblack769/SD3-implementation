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
class IntersectFinder;
class CompressedSet{
    /*
    use:
        Efficient bit operations on arbitrary integers

        The idea is you add, set individual bits, and then
        you can perform efficient basic binary bitwise operations across
        CompressedBit instances, and also can get fast summary statistics like
        `count` and `any`.

    implementation:
        Uses BlockSet to fit all bits in one x/BLOCK_SIZE
        equivalence class.

        Uses efficient arbitrary maps (should use hash tables, right now BST)
        to contains many BlockSets across a wide range of integers

        &= and |= are implemented as set intersection and union, where only the middle
        uses actual bit operations to find the outcome.
    */
protected:
    typedef typename map<int64_t,BasicBlockSet>::iterator set_iterator;
    typedef typename map<int64_t,BasicBlockSet>::const_iterator const_set_iterator;
    map<int64_t,BasicBlockSet> data;
public:
    friend class IntersectFinder;

    void add(int64_t element);
    void add_block(int64_t start,int64_t size);
    bool has(int64_t element);
    void intersect(CompressedSet & outer);
    bool has_any_in_intersect(CompressedSet & outer);
    void unite(CompressedSet & outer);
    void unite_block(int64_t block_loc, BasicBlockSet & outer);
    BasicBlockSet & get_block(int64_t block_loc);
    void subtract(CompressedSet & outer);//boolean operator (x & !y)
    bool any();
    void clear();
    void swap(CompressedSet & other){ this->data.swap(other.data); }
    int64_t count();
    int64_t count_intersect(CompressedSet & outer);
    friend ostream & operator << (ostream & os, CompressedSet &bitset);
protected:
    void and_with_optional_neg(CompressedSet & outer,bool neg);
};
extern CompressedSet empty_set;
