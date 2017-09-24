#ifndef __HISTORYPOINTTABLE_H__
#define __HISTORYPOINTTABLE_H__

#include "Point.h"
#include <map>

using namespace std;

class HistoryPointTable {

  public:
    HistoryPointTable(){};

    void addNewPoint(long long memAddress, long long PC, long long numAccesses = 1, long long accessSize = 4,
                     MemAccessMode readOrWrite = READ);

    bool doesPointExist(long long memAddress);

    list<Point *> getPoints(long long memAddress);

    map<Interval, bool> getIntervalMap();

  private:
    std::multimap<long long, Point *> myPoints;
};

#endif
