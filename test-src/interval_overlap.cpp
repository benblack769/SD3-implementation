#include "test_object.h"
#include "dynamic_gcd.h"
#include "IntervalOverlap.h"

template<class IntTy1,class IntTy2>
vector<pair<IntTy1,IntTy2> > naive_check_overlap(vector<IntTy1> intervals1, vector<IntTy2> intervals2){
    //DOESN'T WORK!!!!!!!!!!!!!!!!!!!!!!!!!
    vector<pair<Block,Block> > overlap;
    for(int int1 = 0; int1 < intervals1.size(); int1++){
        for(int int2 = 0; int2 < intervals2.size(); int2++){
            if(is_overlap(intervals1[int1],intervals2[int2])){
                overlap.push_back(make_pair(intervals1[int1],intervals2[int2]));
            }
        }
    }
    return overlap;
}
template<class IntTy1,class IntTy2>
bool overlaps_equal(vector<pair<IntTy1,IntTy2> > one, vector<pair<IntTy1,IntTy2> > other){
    //DOESN'T WORK!!!!!!!!!!!!!!!!!!!!!!!!!
    if(one.size() != other.size()){
        return false;
    }
    size_t size = one.size();
    for(int i = 0; i < size; i++){
        bool flag = false;
        for(int j = 0; j < size; i++){
            if(one[i].first == other[j].first && one[i].second == other[j].second){
                flag = true;
            }
        }
        if(!flag){
            return false;
        }
    }
    return true;
}

template<class IntervalType>
inline void print_is_in_from_zero(IntervalType interval) {
    //allows visual inspection of strides
    cout << interval << endl;
    for (int i = 0; i < interval.end(); i++) {
        if (interval.is_in(i)) {
            cout << "X";
        } else {
            cout << "O";
        }
    }
    cout << endl;
}

void test_overlap(){
    vector<Block> vec1;
    vec1.push_back(Block(1,2));
    vec1.push_back(Block(3,8));
    vec1.push_back(Block(12,13));
    vec1.push_back(Block(2,5));
    vector<Block> vec2;
    vec2.push_back(Block(0,1));
    vec2.push_back(Block(1,3));
    vec2.push_back(Block(20,21));
    vec2.push_back(Block(4,5));
    vec2.push_back(Block(4,5));
    cout << "first vector" << endl;
    for(int i = 0; i < vec1.size(); i++){
        print_is_in_from_zero(vec1[i]);
    }
    cout << "second vector" << endl;
    for(int i = 0; i < vec2.size(); i++){
        print_is_in_from_zero(vec2[i]);
    }
    vector<pair<Block,Block> > overlap = check_overlap(vec1,vec2);
    vector<pair<Block,Block> > naive_overlap = naive_check_overlap(vec1,vec2);

    cout << "number overlap = " << naive_overlap.size() << endl;
    for(int i = 0; i < overlap.size(); i++){
        cout << "overlap "  << i << endl;
        cout << "first: " << overlap[i].first << endl;
        cout << "second: " << overlap[i].second << endl;
    }
    t_assert(overlaps_equal(overlap,naive_overlap));
}
