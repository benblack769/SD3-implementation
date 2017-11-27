#include <cassert>
#include "CompressedBits.h"

CompressedSet empty_set;

void BlockSet::add(int64_t element){
    assert(element < BLOCK_SIZE && element >= 0);
    bits[element] = true;
}
bool BlockSet::has(int64_t element){
    assert(element < BLOCK_SIZE && element >= 0);
    return bits[element];
}
void BlockSet::operator |= (const BlockSet &outer){
    bits |= outer.bits;
}
void BlockSet::operator &= (const BlockSet &  outer){
    bits &= outer.bits;
}
void BlockSet::subtract(const BlockSet & outer){
    bits &= ~outer.bits;
}
bool BlockSet::any(){
    return bits.any();
}
int64_t BlockSet::count(){
    return bits.count();
}
bool BlockSet::contains(const BlockSet &  outer){
    return (bits | outer.bits) == bits;
}


void CompressedSet::add(int64_t element){
    data[uint64_t(element) / BLOCK_SIZE].add(uint64_t(element) % BLOCK_SIZE);
}

void CompressedSet::add_block(int64_t element,int64_t size){
    for(int64_t i = 0; i < size; i++){
        this->add(element+i);
    }
}
bool CompressedSet::has(int64_t element){
    return data[uint64_t(element) / BLOCK_SIZE].has(uint64_t(element) % BLOCK_SIZE); 
}

bool CompressedSet::has_all_block(int64_t element,int64_t size){
    for(int64_t i = 0; i < size; i++){
        if(!has(i+element)){
            return false;
        }
    }
    return true;
}
bool CompressedSet::has_any_in_block(int64_t element,int64_t size){
    for(int64_t i = 0; i < size; i++){
        if(has(i+element)){
            return true;
        }
    }
    return false;
}
void CompressedSet::intersect(CompressedSet & outer){
    for(set_iterator iter = data.begin(); iter != data.end(); ){
        int64_t key = iter->first;
        BlockSet & value = iter->second;
        if(outer.data.count(key)){
            value &= outer.data[key];
            if(!value.any()){
                //be careful, erases current element. 
                data.erase(iter++);
            }
            else{
                ++iter;
            }
        }else{
            //be careful, erases current element.
            data.erase(iter++);
        }
    }
}
bool CompressedSet::has_any_in_intersect(CompressedSet & outer){
    if(outer.data.size() < this->data.size()){
        outer.has_any_in_intersect(*this);
    }
    for(set_iterator iter = data.begin(); iter != data.end(); ++iter){
        int64_t key = iter->first;
        BlockSet value = iter->second;
        if(outer.data.count(key) & (value.bits & outer.data[key].bits).any()){
            return true;
        }
    }
    return false;
}
bool CompressedSet::contains(CompressedSet & outer){
    for(set_iterator iter = outer.data.begin(); iter != outer.data.end(); ++iter){
        int64_t key = iter->first;
        if(data.count(key) && !data[key].contains(iter->second)){
            return false;
        }
    }
    return true;
}

void CompressedSet::subtract(CompressedSet & outer){
    for(set_iterator iter = data.begin(); iter != data.end(); ){
        int64_t key = iter->first;
        BlockSet & value = iter->second;
        if(outer.data.count(key)){
            value.subtract(outer.data[key]);
            if(!value.any()){
                //be careful, erases current element. 
                data.erase(iter++);
            }
            else{
                ++iter;
            }
        }else{
            ++iter;
        }
    }
}
void CompressedSet::unite(CompressedSet & outer){
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

bool CompressedSet::any(){
    for(set_iterator iter = data.begin(); iter != data.end(); ++iter){
        if(iter->second.any()){
            return true;
        }
    }
    return false;
}
int64_t CompressedSet::count(){
    int64_t sum = 0;
    for(set_iterator iter = data.begin(); iter != data.end(); ++iter){
        sum += iter->second.count();
    }
    return sum;
}
void CompressedSet::clear(){
    data.clear();
}
