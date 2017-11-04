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
protected:
    //BlockType type;
    bitset<BLOCK_SIZE> bits;
    //vector<uint8_t> bits;
public:
    BlockSet(){}
    void add(int64_t element);
    void operator |= (BlockSet outer);
    void operator &= (BlockSet outer);
    bool any();
    int64_t count();
};

class CompressedBits{
protected:
    typedef typename map<int64_t,BlockSet>::iterator set_iterator;
    map<int64_t,BlockSet> data;
public:
    void add(int64_t element);
    void operator &=(CompressedBits & outer);
    void operator|=(CompressedBits & outer);
    bool any();
    int64_t count();
};
