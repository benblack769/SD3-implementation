#ifndef __HISTORYPOINTTABLE_H__
#define __HISTORYPOINTTABLE_H__

#include "Types.h"
#include "Point.h"
#include <map>

using namespace std;

class HistoryPointTable {

  public:
    HistoryPointTable(){};

    void addNewPoint(int64_t memAddress, int64_t PC, int64_t numAccesses = 1, int64_t accessSize = 4,
                     MemAccessMode readOrWrite = READ);

    bool doesPointExist(int64_t memAddress);

    list<Point *> getPoints(int64_t memAddress);

    map<Interval, bool> getIntervalMap();

  private:
    std::multimap<int64_t, Point *> myPoints;
};

#endif
