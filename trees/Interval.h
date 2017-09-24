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
    //  Interval(long long lowerBound, long long upperBound, IntervalType type,
    //  Point *point=NULL, Stride *stride=NULL);
    Interval(long long lowerBound, long long upperBound);

  private:
    long long myLowerBound;
    long long myUpperBound;
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
