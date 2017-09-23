#ifndef PENDINGPOINTTABLE_H
#define PENDINGPOINTTABLE_H


#include "Point.h"
#include <map>

using namespace std;

class PendingPointTable
{	     
   
 public:
  PendingPointTable(){};

  void addNewPoint(long long memAddress, long long PC, long long numAccesses=1, long long accessSize=4, MemAccessMode readOrWrite=READ); 

  bool updateExistingPoint(long long memAddress, long long PC, MemAccessMode readOrWrite);
  
  bool doesPointExist(long long memAddress, long long PC, MemAccessMode readOrWrite=READ);

  bool isPointKilled(long long memAddress);


 private:

  struct PointEntry{
    Point *point;
    bool killed;

    PointEntry(){
      point = NULL;
      killed = false;
    };
  };

  std::multimap<long long, PointEntry> myPendingPoints;

};

#endif 
