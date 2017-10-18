#include <iostream>
#include "dynamic_gcd.h"

int64_t gcd(int64_t a, int64_t b){
    int64_t max_v = max(a,b);
    int64_t min_v = min(a,b);
    while(min_v != 0){
        int64_t new_min = max_v % min_v;
        max_v = min_v;
        min_v = new_min;
    }
    return max_v;
}
int64_t num_conflict_b(Block one, Block other){
    int64_t overlap_one = other.end - one.begin;
    int64_t overlap_two = one.end - other.begin;
    int64_t items_overlap = min(min(overlap_one,overlap_two),int64_t(0));
    
    int64_t tot_access = one.accesses + other.accesses;
    
    int64_t total_length = one.length() + other.length();
    
    return ceil_div(tot_access * items_overlap,total_length);
}

int64_t num_overlap(SparseStride one, SparseStride other){
    //ensures one.first < other.first always
    if(one.first() > other.first()){
        SparseStride tmp = one;
        one = other;
        other = tmp;
    }
    int64_t delta = other.first() - one.first();
    int64_t overlap_size = one.last() - other.first();
    
    if(overlap_size < 0){
       return 0;
    }
    if(one.stride() == other.stride()){
        if(delta % one.stride() == 0){
            return overlap_size / one.stride(); 
        }
        else{
            return 0;
        }
    }
    else if(one.stride()%other.stride() == 0 || other.stride()%one.stride() == 0){
        
    }
    else{
        //TODO: this currently does not implement gcd test corretly. gives a worst case upper bound analysis
        // we can get away with bad solutions here because there aren't many real world cases where we have 
        //overlapping strides of differnet lengths that do not divide each othe
        return 0; 
    }
    return -1;
}
bool SparseStride::is_in_stride(int access){
    int64_t begin_offset = access - first();
    if(begin_offset < 0){
        return false;
    }
    int64_t stride_offset = begin_offset % stride();
    return stride_offset < block_size();
}

std::ostream& operator<<(std::ostream& os, const SparseStride& obj)
{
      os << " { 'F':" << obj.first() <<
          ", 'L':" << obj.last() << 
            ", 'S':" << obj.stride() << 
            ", 'BS':" << obj.block_size() << " } ";
      return os;
}


void SparseStride::print_in_stride_from_zero(){
    cout << "stride: " << *this << endl;
    for(int i = 0; i < end(); i++){
        if(is_in_stride(i)){
            cout << "X";
        }
        else{
            cout << "O";
        }
    }
    cout << endl;
}
