#include "HistoryPointTable.h"


void HistoryPointTable::addNewPoint(long long memAddress, long long PC, long long numAccesses, 
				    long long accessSize, MemAccessMode readOrWrite)
{
  Point* temp;
  temp = new Point(memAddress, PC, numAccesses, accessSize, readOrWrite);

  myPoints.insert(pair<long long, Point*>(memAddress, temp));
} 

bool HistoryPointTable::doesPointExist(long long memAddress, long long PC)
{
  pair<multimap<long long, Point*>::iterator, multimap<long long, Point*>::iterator> rangeIter;
  multimap<long long, Point*>::iterator iter;
  
  rangeIter = myPoints.equal_range(memAddress);
  
  for(iter = rangeIter.first; iter != rangeIter.second; iter++){
    if(PC == iter->second->getPC()){
 	return true;
    }
  }
  
  return false;
}

