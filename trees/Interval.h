#ifndef __INTERVAL_H__
#define __INTERVAL_H__

using namespace std;

enum IntervalType {
    UndefinedInteraval = -1,
    PointInterval = 0,
    StrideInterval = 1
};

class Interval {
    friend class IntervalLessThan;

  public:
    Interval();
    //  Interval(int64_t lowerBound, int64_t upperBound, IntervalType type,
    //  Point *point=NULL, Stride *stride=NULL);
    Interval(int64_t lowerBound, int64_t upperBound);

  private:
    int64_t myLowerBound;
    int64_t myUpperBound;
    //  IntervalType myType;

    //  Point * myPoint;
    // Stride *myStride;
};

class IntervalLessThan {
  public:
    bool operator()(const Interval &i1, const Interval &i2) const {
        if (i1.myLowerBound < i2.myLowerBound)
            return true;
        else if (i1.myLowerBound == i2.myLowerBound) {
            return i1.myUpperBound < i2.myUpperBound;
        } else
            return false;
    }
};

#endif
