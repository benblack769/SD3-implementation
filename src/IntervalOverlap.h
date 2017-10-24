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
inline void sort_by_first(vector<IntTy> & intervals){
    sort(intervals.begin(),intervals.end(), FirstLessThan<IntTy,IntTy>());
}

template<class IntTy>
inline void sort_by_last(vector<IntTy> & intervals){
    sort(intervals.begin(),intervals.end(), LastLessThan<IntTy,IntTy>());
}
inline bool is_between_inclusive(int64_t in,int64_t low,int64_t high){
    return in >= low && in <= high;
}

template<class IntTy1,class IntTy2>
inline bool is_overlap(IntTy1 one, IntTy2 other){
    return is_between_inclusive(one.first(),other.first(),other.last()) ||
           is_between_inclusive(one.last(), other.first(),other.last());
}

template<class IntTy1,class IntTy2>
inline vector<pair<IntTy1,IntTy2> > check_overlap(vector<IntTy1> intervals1, vector<IntTy2> intervals2){
    sort_by_first(intervals1);
    vector<IntTy2> sorted_first = intervals2;
    vector<IntTy2> sorted_last = intervals2;
    sort_by_first(sorted_first);
    sort_by_last(sorted_last);
    
    vector<pair<IntTy1,IntTy2> > overlap;
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
            for(int k = ilast; k < s_last; k++){
                cout << "added_last" << item << sorted_last[k] << "   " << k << endl;
                overlap.push_back(make_pair(item,sorted_last[k]));
            }
            for(int k = ifirst; k < s_first; k++){
                //PERFORMANCE WARNING! O(n^2) performance
                if(find(sorted_last.begin()+ilast,sorted_last.begin()+s_last,sorted_first[k]) == sorted_last.begin()+s_last){
                    cout << "added_first" << item << sorted_first[k] << endl;
                    overlap.push_back(make_pair(item,sorted_first[k]));
                }
            }
        }
    }
    return overlap;
}
