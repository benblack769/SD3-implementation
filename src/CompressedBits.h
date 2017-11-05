//#include <vector>
#include <map>
#include <bitset>
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
    void operator |= (BlockSet outer);
    void operator &= (BlockSet outer);
    bool any();
    int64_t count();
};

class CompressedBits{
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
    map<int64_t,BlockSet> data;
public:
    void add(int64_t element);
    bool has(int64_t element);
    void operator &=(CompressedBits & outer);
    void operator|=(CompressedBits & outer);
    bool any();
    int64_t count();
};
