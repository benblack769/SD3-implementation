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

    //adds point to table
    bool addNewPoint(int64_t memAddress, int64_t PC, int64_t numAccesses = 1, int64_t accessSize = 4,
                     MemAccessMode readOrWrite = READ);

    // If memory address already is in table and is not killed, updates number
    // of accesses. Returns true if update of number of accesses occurs.
    //bool updateExistingPoint(int64_t memAddress, int64_t PC, MemAccessMode readOrWrite);

    //bool doesPointExist(int64_t memAddress, int64_t PC, MemAccessMode readOrWrite = READ);

    //bool isPointKilled(int64_t memAddress);

    //  list<Dependence> checkForPointDependences(HistoryPointTable ht);

    //  map<Interval, Point *> getIntervalMap();

    //joins tables. does not do anything with killed bits
    void merge(PendingPointTable other);

    void print();

    //sorted_elements
    vector<PointerInterval<Point *> > intervals(MemAccessMode access_mode);

  private:
    access_mode_pair<multi_map<int64_t, Point *>> myPendingPoints;
};

#endif
