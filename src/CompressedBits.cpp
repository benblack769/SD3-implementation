#include <cassert>
#include "CompressedBits.h"

BasicBlockSet empty_bb_set;

ostream & operator << (ostream & os, const BasicBlockSet &block_set){
    for(size_t i = 0; i < BLOCK_SIZE; i++){
        os << block_set.bits[i];
    }
    return os;
}
