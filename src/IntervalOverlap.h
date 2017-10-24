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
/*
template<class IntTy1,class IntTy2>
struct LastLessThan {
    bool operator()(const IntTy1 &i1, const IntTy2 &i2) const {
        return i1.last() < i2.last();
    }
};*/


template<class IntTy>
inline void sort_by_first(vector<IntTy> & intervals){
    sort(intervals.begin(),intervals.end(), FirstLessThan<IntTy,IntTy>());
}
/*
template<class IntTy>
inline void sort_by_last(vector<IntTy> & intervals){
    sort(intervals.begin(),intervals.end(), LastLessThan<IntTy,IntTy>());
}*/
inline bool is_between_inclusive(int64_t in,int64_t low,int64_t high){
    return in >= low && in <= high;
}

template<class IntTy1,class IntTy2>
inline bool is_overlap(IntTy1 one, IntTy2 other){
    return is_between_inclusive(one.first(),other.first(),other.last()) ||
           is_between_inclusive(one.last(), other.first(),other.last());
}
/*
check_overlap(A,B)
    sort A by first()
    sort B by first()
    overlap = Set()
    before_last = vector()
    ib = 0
    for interval in A
        new_before_last = vector()
        for item in before_last:
            if item.last() >= interval.first():
                new_before_last.append(item)
        swap(before_last,new_before_last)
        
        increment ib until B[ib].first() >= interval.first()
            if B[ib].last() >= interval.first()
                before_last.append(B[ib])
                
        for bl in before_last
            overlap.add(bl,interval)
            
        for ib2 from ib until B[ib2].first() > interval.last()
            overlap.add(B[ib2],interval)

The idea here is that the queue holds things which 
begin before the interval starts and end after
the start of the interval.

Then it adds the things which start after the interval starts
(which you need B sorted to efficiently do).

So by adding the things in the queue to the things you know start
before the interval ends, then you get all of the conflicts

step 1: Removes things that do not overlap from the queue
step 2: Adds items that start before interval.first() and conflict with interval 
        to the queue
step 3: adds all items in queue to the conflicts
step 4: Items that start after interval starts and conflict to the overlap

Overall, you get each conflict exactly once. 
*/
template<class IntTy1,class IntTy2>
inline vector<pair<IntTy1,IntTy2> > check_overlap(vector<IntTy1> intervals1, vector<IntTy2> intervals2){
    sort_by_first(intervals1);
    sort_by_first(intervals2);
    //priority_queue<IntTy2,LastLessThan<IntTy2,IntTy2>> queue;

    vector<pair<IntTy1,IntTy2> > overlap;
    size_t ib = 0;
    size_t size_2 = intervals2.size();
    vector<IntTy2> before_last;
    vector<IntTy2> new_before_last;

    for(int int1 = 0; int1 < intervals1.size(); int1++){
        IntTy1 interval = intervals1[int1];
        
        new_before_last.clear();
        for(size_t i = 0; i < before_last.size(); i++){
            if(before_last[i].last() >= interval.first()){
                new_before_last.push_back(before_last[i]);
            }
        }
        swap(before_last,new_before_last);
        
        for(; ib < size_2 && intervals2[ib].first() < interval.first(); ib++){
            if(intervals2[ib].last() >= interval.first()){
                before_last.push_back(intervals2[ib]);
            }
        }
        
        for(size_t i = 0; i < before_last.size(); i++){
            overlap.push_back(make_pair(interval,before_last[i]));
        }
        
        for(size_t ib2 = ib; ib2 < size_2 && intervals2[ib2].first() <= interval.last(); ib2++){
            overlap.push_back(make_pair(interval,intervals2[ib2]));
        }
    }
    return overlap;
}
//ARG!!! It turns out this function is broken. Keeping around for refrence only
/*template<class IntTy1,class IntTy2>
inline vector<pair<IntTy1,IntTy2> > check_overlap_broken(vector<IntTy1> intervals1, vector<IntTy2> intervals2){
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
                overlap.push_back(make_pair(item,sorted_last[k]));
            }
            for(int k = ifirst; k < s_first; k++){
                //PERFORMANCE WARNING! O(n^2) performance
                if(find(sorted_last.begin()+ilast,sorted_last.begin()+s_last,sorted_first[k]) == sorted_last.begin()+s_last){
                    overlap.push_back(make_pair(item,sorted_first[k]));
                }
            }
        }
    }
    return overlap;
}
*/
