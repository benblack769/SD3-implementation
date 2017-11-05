//#include "Interval.h"
#include <algorithm>
#include <vector>
#include "dynamic_gcd.h"

using namespace std;

/*
check_overlap algorithm:

Need: we need to be able to fine overlaps between sets of strides efficiently. 
This fits that need by finding overlaps between vectors of generic intervals, which can 
be strides. 
  
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

The idea here is that the before_last list holds things which
begin before the interval starts and end after
the start of the interval.

Then it adds the things which start after the interval starts
(which you need B sorted to efficiently do).

So by adding the things in the before_last list to the things you know start
before the interval ends, then you get all of the conflicts

step 1: Removes things that do not overlap from before_last
step 2: Adds items to before_last that start before interval.first() and
        conflict with interval
step 3: Adds all items in before_last to the conflicts
step 4: Items that start after interval starts and conflict to the overlap

Overall, you get each conflict exactly once.
*/


/*
PointerInterval and CachedInterval are possible wrappers over 
regular interval types like 
*/
template<class IntervalType>
class PointerInterval{
protected:
    IntervalType * ptr;
public:
    PointerInterval(IntervalType * in_ptr):
        ptr(in_ptr){}
    int64_t first(){return ptr->first();}
    int64_t last(){return ptr->last();}
};

template<class IntervalType>
class CachedInterval:
    public Block{
protected:
    IntervalType * ptr;
public:
    CachedInterval(IntervalType * in_ptr):
        Block(in_ptr->first(),in_ptr->end()){}
};

template<class IntTy1,class IntTy2>
struct FirstLessThan {
    bool operator()(const IntTy1 &i1, const IntTy2 &i2) const {
        return i1.first() < i2.first();
    }
};

template<class IntTy>
inline void sort_by_first(vector<IntTy> & intervals){
    sort(intervals.begin(),intervals.end(), FirstLessThan<IntTy,IntTy>());
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
    sort_by_first(intervals2);

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
