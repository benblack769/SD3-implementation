#include "PendingPointTable.h"

  // If memory address isn't killed, adds information about this access to table.  Returns
  // true if information is added, false if memory address is already killed.  If this
  // access is a write, the point is turned to "killed"
bool PendingPointTable::addNewPoint(long long memAddress, long long PC, long long numAccesses, 
				    long long accessSize, MemAccessMode readOrWrite)
{
  // If memory address already killed, do not add new elements.
  map<long long, PointEntry>::iterator iter = myPendingPoints.find(memAddress);
  if(iter != myPendingPoints.end()){
    // Memory address already in table
    if(iter.second.killed == true){
      return false;
    }
    else{
      Point *p = new Point(memAddress, PC, numAccesses, accessSize, readOrWrite);
      iter.second.points.insert(p);
    }
  }
  else{
    PointEntry temp;
    temp.points.insert(new Point(memAddress, PC, numAccesses, accessSize, readOrWrite));
    myPendingPoints.insert(pair<long long, PointEntry>(memAddress, temp));
  }
 
  if(readOrWrite == WRITE){
    temp.killed = true;
  }
  return true;
} 

bool PendingPointTable::doesPointExist(long long memAddress, long long PC, MemAccessMode readOrWrite) 
{
  map<long long, PointEntry>::iterator iter = myPendingPoints.find(memAddress); 

  if(iter != myPendingPoints.end()){
    for(list<Point*>::iterator liter = iter.second.points.begin(); liter != iter.second.points.end(); liter++){
      if(PC == liter->getPC() && readOrWrite == liter->getAccessMode()){
	return true;
      }
    }
  }
  
  return false;
}

// If memory address already is in table and is not killed, updates number of accesses.
// Returns true if update of number of accesses occurs.
bool PendingPointTable::updateExistingPoint(long long memAddress, long long PC, MemAccessMode readOrWrite)
{

  map<long long, PointEntry>::iterator iter = myPendingPoints.find(memAddress); 

  if(iter != myPendingPoints.end()){
    if(iter.second.killed == false){
      return false;
    }

    for(list<Point*>::iterator liter = iter.second.points.begin(); liter != iter.second.points.end(); liter++){
      if(PC == liter->getPC() && readOrWrite == liter->getAccessMode()){
	liter->addAccess();
	return true;
      }
    }
  }
  
  return false;
}

bool PendingPointTable::isPointKilled(long long memAddress)
{
  map<long long, PointEntry>::iterator iter = myPendingPoints.find(memAddress); 

  if(iter != myPendingPoints.end()){
    if(iter.second.killed == false){
      return true;
    }
  }

  return false;
}


list<Dependences> PendingPointTable::checkForPointDependences(HistoryPointTable ht)
{
  list<Dependence> conflicts;

  map<long long, PointEntry>::iterator iter = myPendingPoints.begin();
  for( ; iter!= myPendingParts.end(); iter++){
    for(list<Point*>::iterator liter = iter.second.points.begin(); liter != iter.second.points.end(); liter++){
      list<Point *> oldPoints = ht->getPoints(liter->getMemAddress());

      MemAccessMode pendMode = liter->getAccessMode();
      for(list<Point*>::iterator piter = oldPoints.begin(); piter != oldPoints.end(); piter++){
	MemAccessMode oldMode = piter->getAccessMode();
	if(oldMode != READ && pendMode != READ){
	  Dependence d(liter->getMemoryAddr(), piter->getPC(), oldMode, liter->getPC(), pendMode);
	  conflicts.insert(d);
	}

      }
    }
  }
  return conflicts;
}


map<Interval, PointEntry> PendingPointTable::getIntervalMap()
{
  map<Interval, PointEntry, IntervalLessThan> iMap;

  map<long long, PointEntry>::iterator iter = myPendingPoints.begin();
  
  for( ; iter != myPendingPoints.end(); iter++){
    Interval i(iter.second->getMemoryAddr(), iter.second->getMemoryAddr());
    iMap[i] = iter.second;
  }
  return iMap;
}

