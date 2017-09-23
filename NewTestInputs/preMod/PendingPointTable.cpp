#include "PendingPointTable.h"


void PendingPointTable::addNewPoint(long long memAddress, long long PC, long long numAccesses, 
				    long long accessSize, MemAccessMode readOrWrite)
{
  PointEntry temp;
  temp.point = new Point(memAddress, PC, numAccesses, accessSize, readOrWrite);
  if(readOrWrite == WRITE){
    temp.killed = true;
  }
  myPendingPoints.insert(pair<long long, PointEntry>(memAddress, temp));
} 

bool PendingPointTable::doesPointExist(long long memAddress, long long PC, MemAccessMode readOrWrite) // This is the only thing called in PointOrStride...
{
  pair<multimap<long long, PointEntry>::iterator, multimap<long long, PointEntry>::iterator> rangeIter;
  multimap<long long, PointEntry>::iterator iter;
  
  rangeIter = myPendingPoints.equal_range(memAddress);
  
  for(iter = rangeIter.first; iter != rangeIter.second; iter++){
    if(PC == iter->second.point->getPC() && readOrWrite == iter->second.point->getAccessMode()){
      return true;
    }
  }
  
  return false;
}

bool PendingPointTable::updateExistingPoint(long long memAddress, long long PC, MemAccessMode readOrWrite)
{
  pair<multimap<long long, PointEntry>::iterator, multimap<long long, PointEntry>::iterator> rangeIter;
  multimap<long long, PointEntry>::iterator iter;
  
  rangeIter = myPendingPoints.equal_range(memAddress);
  
  for(iter = rangeIter.first; iter != rangeIter.second; iter++){
    if(iter->second.killed == true){
      return false;
    }

    if(PC == iter->second.point->getPC() && readOrWrite == iter->second.point->getAccessMode()){
      iter->second.point->addAccess();
      return true;
    }
  }
  
  return false;
}

bool PendingPointTable::isPointKilled(long long memAddress)
{
  pair<multimap<long long, PointEntry>::iterator, multimap<long long, PointEntry>::iterator> rangeIter;
  multimap<long long, PointEntry>::iterator iter;
  
  rangeIter = myPendingPoints.equal_range(memAddress);
  
  for(iter = rangeIter.first; iter != rangeIter.second; iter++){
    if( iter->second.killed == true){
 	return true;
    }
  }
  
  return false;
}


