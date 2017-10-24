#include <vector>
#include <map>
#include <bitset>
#include "Types.h"

using namespace std;

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
    void add_access(uint8_t element){
        bits[element] = true;
    }
    void merge_into_except (BlockSet outer,BlockSet except){
        bits |= outer.bits & except;
    }
    void merge_into (BlockSet outer){
        bits |= outer.bits;
    }
    bool has_overlap(BlockSet other){
        return (other.bits & bits).any();
    }
};

class CompressedBits{
protected:
    typedef typename map<int64_t,BlockSet>::iterator set_iterator;
    map<int64_t,BlockSet> data;
public:
    void add_access(int64_t element){
        data[element / BLOCK_SIZE].add_access(element % BLOCK_SIZE);
    }
    void merge_into_except (CompressedBits outer,CompressedBits except){
        for(set_iterator iter = data.begin(); iter != data.end(); ++iter){
            int64_t key = iter->first;
            if(outer.data.count(key)){
                if(outer.data.count(key)){
                    iter->second.outer.merge_into_except(data.find(key)->second);
                }
                else{
                    
                }
            }
        }
    }
    bool has_overlap(CompressedBits outer){
        for(set_iterator iter = data.begin(); iter != data.end(); ++iter){
            int64_t key = iter->first;
            if(outer.data.count(key)){
                if(iter->second.outer.has_overlap(data.find(key)->second)){
                    return true;
                }
            }
        }
        return false;
    }
};
