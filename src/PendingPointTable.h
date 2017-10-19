#ifndef PENDINGPOINTTABLE_H
#define PENDINGPOINTTABLE_H

#include "Point.h"
#include "Types.h"
#include <iostream>
#include <list>
#include <map>

using namespace std;

class PendingPointTable {

  public:
    PendingPointTable(){};

    // If memory address isn't killed, adds information about this access to
    // table.  Returns true if information is added, false if memory address is
    // already killed.  If this access is a write, the point is turned to
    // "killed"
    bool addNewPoint(int64_t memAddress, int64_t PC, int64_t numAccesses = 1, int64_t accessSize = 4,
                     MemAccessMode readOrWrite = READ);

    // If memory address already is in table and is not killed, updates number
    // of accesses. Returns true if update of number of accesses occurs.
    bool updateExistingPoint(int64_t memAddress, int64_t PC, MemAccessMode readOrWrite);

    bool doesPointExist(int64_t memAddress, int64_t PC, MemAccessMode readOrWrite = READ);

    bool isPointKilled(int64_t memAddress);

    //  list<Dependence> checkForPointDependences(HistoryPointTable ht);

    //  map<Interval, PointEntry> getIntervalMap();

    void print();

  private:
    struct PointEntry {
        list<Point *> points;
        bool killed;

        PointEntry() { killed = false; };
    };

    map<int64_t, PointEntry> myPendingPoints;
};

#endif
