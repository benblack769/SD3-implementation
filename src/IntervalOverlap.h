#include "Interval.h"
#include <algorithm>
#include <vector>

using namespace std;

template<class IntTy1,class IntTy2>
struct FirstLessThan {
    bool operator()(const IntTy1 &i1, const IntTy2 &i2) const {
        return i1.first() < i2.first();
    }
};
template<class IntTy1,class IntTy2>
struct LastLessThan {
    bool operator()(const IntTy1 &i1, const IntTy2 &i2) const {
        return i1.last() < i2.last();
    }
};


template<class IntTy>
void sort_by_first(vector<IntTy> & intervals){
    sort(intervals.begin(),intervals.end(), FirstLessThan<IntTy,IntTy>);
}

template<class IntTy>
void sort_by_last(vector<IntTy> & intervals){
    sort(intervals.begin(),intervals.end(), LastLessThan<IntTy,IntTy>);
}

template<class IntTy1,class IntTy2>
inline vector<pair<IntTy1,IntTy2>> check_overlap(vector<IntTy1> intervals1, vector<IntTy2> intervals2){
    sort_by_first(intervals1);
    vector<IntTy2> sorted_first = intervals2;
    vector<IntTy2> sorted_last = intervals2;
    sort_by_first(sorted_first);
    sort_by_last(sorted_last);
    
    vector<pair<IntTy1,IntTy2>> overlap;
    size_t ifirst = 0;
    size_t ilast = 0;
    size_t size_1 = intervals1.size();
    size_t size_2 = intervals2.size();
    
    for(int int1 = 0; int1 < size_1; int1++){
        IntTy1 item = intervals1[int1];
        
        // move ifirst and ilast pointers up to at least item.first()
        for(; ifirst < size_2 && sorted_first[ifirst].first() < item.first(); ifirst++);
        for(; ilast < size_2 && sorted_last[ilast].last() < item.first(); ilast++);
        
        //get pointers to end of conflicts
        size_t s_first = ifirst;
        size_t s_last = ilast;
        for(; s_first < size_2 && sorted_first[s_first].first() <= item.last(); s_first++);
        for(; s_last < size_2 && sorted_last[s_last].last() <= item.last(); s_last++);
        
        //merge two lists of conflicts for unique elements
        if(ifirst != s_first || ilast != s_last){
            //size_t max_union_size = s_first + s_last - ifirst - ilast;
            //vector<IntTy2> union_result(max_union_size);
            //vector<IntTy2> last_conflicts(sorted_last.begin() + ilast, sorted_last.begin() + s_last);            
            //sort_by_first(last_conflicts);
            //set_union(last_conflicts.begin(),last_conflicts.end(),
            //          sorted_first.begin()+ifirst,sorted_first.begin()+s_first,)
            for(int k = ilast; k < s_last; k++){
                overlap.push_back(make_pair(item,sorted_last[k]));
            }
            for(int k = ifirst; k < s_first; k++){
                //PERFORMANCE WARNING! O(n^2) performance
                if(find(sorted_last.begin(),sorted_last.end(),sorted_first[k]) != sorted_last.end()){
                    overlap.push_back(make_pair(item,sorted_last[k]));
                }
            }
        }
    }
    return overlap;
}
