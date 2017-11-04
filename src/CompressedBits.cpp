#include <cassert>
#include "CompressedBits.h"

void BlockSet::add(int64_t element){
    assert(element < BLOCK_SIZE);
    bits[element] = true;
}
void BlockSet::operator |= (BlockSet outer){
    bits |= outer.bits;
}
void BlockSet::operator &= (BlockSet outer){
    bits &= outer.bits;
}
bool BlockSet::any(){
    return bits.any();
}
int64_t BlockSet::count(){
    return bits.count();
}


void CompressedBits::add(int64_t element){
    data[element / BLOCK_SIZE].add(element % BLOCK_SIZE);
}
void CompressedBits::operator &=(CompressedBits & outer){
    for(set_iterator iter = data.begin(); iter != data.end(); ++iter){
        int64_t key = iter->first;
        BlockSet & value = iter->second;
        if(outer.data.count(key)){
            value &= outer.data[key];
            if(!value.any()){
                //be careful, erases current element. 
                data.erase(key);
            }
        }else{
            //be careful, erases current element.
            data.erase(key);
        }
    }
}
void CompressedBits::operator|=(CompressedBits & outer){
    for(set_iterator iter = outer.data.begin(); iter != outer.data.end(); ++iter){
        int64_t key = iter->first;
        if(this->data.count(key)){
            this->data[key] |= iter->second;
        }
        else{
            this->data[key] = iter->second;
        }
    }
}
bool CompressedBits::any(){
    for(set_iterator iter = data.begin(); iter != data.end(); ++iter){
        if(iter->second.any()){
            return true;
        }
    }
    return false;
}
int64_t CompressedBits::count(){
    int64_t sum = 0;
    for(set_iterator iter = data.begin(); iter != data.end(); ++iter){
        sum += iter->second.count();
    }
    return sum;
}
