#include "Interval.h"

Interval::Interval()
{
  myLowerBound=myUpperBound=0;
  myType = UndefinedInterval;
  myPoint=NULL;
  myStride=NULL;
}

Interval::Interval(long long lower, long long upper)
{
  myLowerBound=lower;
  myUpperBound=upper;
  myType = UndefinedInterval;
  myPoint=NULL;
  myStride=NULL;
}


Interval::Interval(long long lowerBound, long long upperBound, IntervalType type, Point *point, Stride *stride)
{
  myLowerBound = lowerBound;
  myUpperBound = upperBound;
  myType = type;
  myPoint = point;
  myStride = stride;
}


