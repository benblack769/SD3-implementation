#include "Interval.h"

Interval::Interval() {
    myLowerBound = myUpperBound = 0;
    myType = UndefinedInterval;
    myPoint = NULL;
    myStride = NULL;
}

Interval::Interval(int64_t lower, int64_t upper) {
    myLowerBound = lower;
    myUpperBound = upper;
    myType = UndefinedInterval;
    myPoint = NULL;
    myStride = NULL;
}

Interval::Interval(int64_t lowerBound, int64_t upperBound, IntervalType type, Point *point, Stride *stride) {
    myLowerBound = lowerBound;
    myUpperBound = upperBound;
    myType = type;
    myPoint = point;
    myStride = stride;
}
