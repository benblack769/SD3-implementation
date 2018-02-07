#pragma once
#include <map>
#include <bitset>
#include <ostream>
#include "Types.h"

using namespace std;

//make sure this is a power of 2
#define BLOCK_SIZE 256

//enum BlockType{BitsetBlock,VecBlock};

typedef unsigned char uint8_t;

class BlockSet{
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
    BlockSet(){}
    void add(int64_t element);
    bool has(int64_t element);
    void operator |= (const BlockSet & outer);
    void operator &= (const BlockSet & outer);
    void subtract(const BlockSet &  outer);
    bool any();
    int64_t count();
    //bool contains(const BlockSet &  outer);
    friend class CompressedSet;
    friend ostream & operator << (ostream & os,const BlockSet & bitset);
};

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
    typedef typename map<int64_t,BlockSet>::iterator set_iterator;
    typedef typename map<int64_t,BlockSet>::const_iterator const_set_iterator;
    map<int64_t,BlockSet> data;
public:
    void add(int64_t element);
    void add_block(int64_t start,int64_t size);
    bool has(int64_t element);
    void intersect(CompressedSet & outer);
    bool has_any_in_intersect(CompressedSet & outer);
    void unite(CompressedSet & outer);
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
