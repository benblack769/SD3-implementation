#include <cassert>
#include "CompressedBits.h"

CompressedSet empty_set;
BasicBlockSet empty_bb_set;

ostream & operator << (ostream & os, const BasicBlockSet &block_set){
    for(size_t i = 0; i < BLOCK_SIZE; i++){
        os << block_set.bits[i];
    }
    return os;
}



void CompressedSet::add(int64_t element){
    data[uint64_t(element) / BLOCK_SIZE].add(uint64_t(element) % BLOCK_SIZE);
}

BasicBlockSet & CompressedSet::get_block(int64_t block_loc){
    assert(block_loc % BLOCK_SIZE == 0);
    set_iterator iter = data.find(block_loc);
    return (iter != data.end()) ? iter->second : empty_bb_set;
}
void CompressedSet::unite_block(int64_t block_loc, BasicBlockSet & outer){
    assert(block_loc % BLOCK_SIZE == 0);
    data[block_loc] |= outer;
}
void CompressedSet::add_block(int64_t element,int64_t size){
    for(int64_t i = 0; i < size; i++){
        this->add(element+i);
    }
}
bool CompressedSet::has(int64_t element){
    return data[uint64_t(element) / BLOCK_SIZE].has(uint64_t(element) % BLOCK_SIZE);
}
void CompressedSet::intersect(CompressedSet & outer){
    set_iterator iter = this->data.begin(), outer_iter = outer.data.begin();
    for(; iter != this->data.end() && outer_iter != outer.data.end(); ){
        int64_t this_key = iter->first;
        int64_t outer_key = outer_iter->first;
        if(this_key == outer_key){
            iter->second &= outer_iter->second;
            if(!iter->second.any()){
                data.erase(iter++);
            }
            else{
                ++iter;
            }
            ++outer_iter;
        }
        else if(this_key < outer_key){
            set_iterator new_iter = this->data.lower_bound(outer_key);
            this->data.erase(iter,new_iter);
            iter = new_iter;
        }
        else{
            outer_iter = outer.data.lower_bound(this_key);
        }
    }
    if(iter != this->data.end()){
        this->data.erase(iter,this->data.end());
    }
}
bool CompressedSet::has_any_in_intersect(CompressedSet & outer){
    for(set_iterator iter = this->data.begin(), outer_iter = outer.data.begin();
            iter != this->data.end() && outer_iter != outer.data.end(); ){
        int64_t this_key = iter->first;
        int64_t outer_key = outer_iter->first;
        if(this_key == outer_key){
            BasicBlockSet block = iter->second;
            block &= outer_iter->second;
            if(block.any()){
                return true;
            }
            ++iter;
            ++outer_iter;
        }
        else if(this_key < outer_key){
            iter = this->data.lower_bound(outer_key);
        }
        else{
            outer_iter = outer.data.lower_bound(this_key);
        }
    }
    return false;
}

int64_t CompressedSet::count_intersect(CompressedSet & outer){
    int64_t count = 0;
    for(set_iterator iter = this->data.begin(), outer_iter = outer.data.begin();
            iter != this->data.end() && outer_iter != outer.data.end(); ){
        int64_t this_key = iter->first;
        int64_t outer_key = outer_iter->first;
        if(this_key == outer_key){
            BasicBlockSet intersect = iter->second;
            intersect &= outer_iter->second;
            count += intersect.count();

            ++iter;
            ++outer_iter;
        }
        else if(this_key < outer_key){
            iter = this->data.lower_bound(outer_key);
        }
        else{
            outer_iter = outer.data.lower_bound(this_key);
        }
    }
    return count;
}

void CompressedSet::subtract(CompressedSet & outer){
    for(set_iterator iter = this->data.begin(), outer_iter = outer.data.begin();
            iter != this->data.end() && outer_iter != outer.data.end(); ){
        int64_t this_key = iter->first;
        int64_t outer_key = outer_iter->first;
        if(this_key == outer_key){
            iter->second.subtract(outer_iter->second);
            if(!iter->second.any()){
                data.erase(iter++);
            }
            else{
                ++iter;
            }
            ++outer_iter;
        }
        else if(this_key < outer_key){
            iter = this->data.lower_bound(outer_key);
        }
        else{
            outer_iter = outer.data.lower_bound(this_key);
        }
    }
}
void CompressedSet::unite(CompressedSet & outer){
    set_iterator iter = this->data.begin(), outer_iter = outer.data.begin();
    for(;iter != this->data.end() && outer_iter != outer.data.end(); ){
        int64_t this_key = iter->first;
        int64_t outer_key = outer_iter->first;
        if(this_key == outer_key){
            iter->second |= outer_iter->second;
            ++iter;
            ++outer_iter;
        }
        else if(this_key < outer_key){
            iter = this->data.lower_bound(outer_key);
        }
        else{
            set_iterator new_outer_iter = outer.data.lower_bound(this_key);
            this->data.insert(outer_iter,new_outer_iter);
            outer_iter = new_outer_iter;
        }
    }
    if(outer_iter != outer.data.end()){
        this->data.insert(outer_iter,outer.data.end());
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
ostream & operator << (ostream & os, CompressedSet &compset){
    for(CompressedSet::set_iterator iter = compset.data.begin(); iter != compset.data.end(); ++iter){
        int64_t upper_loc = iter->first;
        BasicBlockSet lower_loc = iter->second;
        os << hex << upper_loc << dec << ": " << lower_loc << "\n";
    }
    return os;
}
