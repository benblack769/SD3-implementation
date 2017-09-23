#ifndef PENDINGPOINTTABLE_H
#define PENDINGPOINTTABLE_H


#include "Point.h"
#include <map>
#include <list>
#include <iostream>

using namespace std;

class PendingPointTable
{	     
   
 public:
  PendingPointTable(){};

  // If memory address isn't killed, adds information about this access to table.  Returns
  // true if information is added, false if memory address is already killed.  If this
  // access is a write, the point is turned to "killed"
  bool addNewPoint(long long memAddress, long long PC, long long numAccesses=1, long long accessSize=4, 
		   MemAccessMode readOrWrite=READ); 

  // If memory address already is in table and is not killed, updates number of accesses.
  // Returns true if update of number of accesses occurs.
  bool updateExistingPoint(long long memAddress, long long PC, MemAccessMode readOrWrite);
  
  bool doesPointExist(long long memAddress, long long PC, MemAccessMode readOrWrite=READ);

  bool isPointKilled(long long memAddress);

  //  list<Dependence> checkForPointDependences(HistoryPointTable ht);

  //  map<Interval, PointEntry> getIntervalMap();

  void print();

 private:

  struct PointEntry{
    list<Point *>points;
    bool killed;

    PointEntry(){
      killed = false;
    };
  };

  map<long long, PointEntry> myPendingPoints;

};

#endif 
