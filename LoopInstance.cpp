#include "LoopIntance.cpp"

LoopInstance::LoopInstance(long long startPC, long long endPC)
{
  myPendingPoints = new PendingPointTable();
  myPendingStrides = new PendingStrideTable();
  myHistoryPoints = new HistoryPointTable();
  myHistoryStrides = new HistoryStridePoint();

  myNumIterations = 0;
  myLoopStartPC = startPC;
  myLoopEndPC = endPC;

  myConflicts = new ConflictTable();

}

void LoopInstance::endCurrentIteration()
{
  // check for point dependences
  list<Dependences> pDep = checkForPointDependences();

  // check for stride dependences
  void checkForStrideDependences();

  // merge pending points into history points
  void foldPendingPointsIntoHistory();

  // merge pending strides into history strides
  void foldPendingStridesIntoHistory();

  // clear pending tables
  
}

void LoopInstance::addMemAccess(long long memAddress, long long PC, int accessSize, MemAccessMode readOrWrite)
{
  pair<multimap<long long, PerPCDetector>::iterator, multimap<long long, PerPCDetector>::iterator> rangeIter;
  multimap<long long, PerPCDetector>::iterator iter;

  rangeIter = myPendingStrides.equal_range(PC);

  for(iter = rangeIter.first; iter != rangeIter.second; iter++){
    if(readOrWrite == iter->second.mode){
      MemAccessType result = iter->second.detector->addAccess(memoryAddr);
      MemAccessType lastType = iter->second.lastType;
      
      if(result == POINT){
	if(myPendingPoints->doesPointExist(memoryAddr, PC)){
	  myPendingPoints->updateExistingPoint(memoryAddr, PC, readOrWrite);
	}
	else{
	  // Create new point
	  myPendingPoints->addNewPoint(memoryAddr, PC, 1, accessSize, readOrWrite);
	}

	if(lastType == UNKNOWN){
	  // If the previous type was UNKNOWN, that means there was an access
	  // to this memory address that was neither classified as a POINT or
	  // a STRIDE and therefore hasn't been accounted for yet.  
	  // This code should only happen when it is a fixed address.
	  myPendingPoints->updateExistingPoint(memoryAddr, PC, readOrWrite);	    
	}
      }
      else if(result == STRIDE){
	int stride = iter->second.detector->getStride();

	if(lastType == UNKNOWN){
	  long long baseAddr = iter->second.detector->getFirstMemAddr();
	  myPendingStrides->addNewStride(PC, baseAddr, memoryAddr, stride, 2, accessSize, readOrWrite);
	}
	else{
	  myPendingStrides->updateExistingStride(PC, memoryAddr, stride, readOrWrite);
	}
      }
      else{
	// This would be UNKNOWN.  It is when we can't make a determination
	// yet perhaps because it is the first element.
      }
    }
    iter->second.lastType = result;
  }

}

void LoopInstance::checkForPointDependences()
{
  map<Interval, PointEntry> toCheck;

  map<Interval, PointEntry> history = myHistoryPointTable->getIntervalMap();
  map<Interval, bool> pending = myPendingPointTable->getIntervalMap();

  map<Interval, bool>::iterator piter = pending.begin();
  map<Interval, bool>::iterator hiter = history.begin();

  for( ; piter != pending.end(); piter++){
    
    long long pendingAddr = piter.first.myLowerBound;

    for( ; hiter != history.end(); hiter++){
      long long historyAddr = hiter.first.myLowerBound;
      
      if(pendingAddr == historyAddr){
	toCheck.insert(piter.first);
      }
      if(pendingAddr > historyAddr){
	break;
      }
    }
  }
    
  // Should have all the intervals that we need to look at again more closely.
  
}
