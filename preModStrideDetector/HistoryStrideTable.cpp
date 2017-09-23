#include "HistoryStrideTable.h"

// This method assumes that a stride does not already exist
void HistoryStrideTable::addNewStride(long long PC, Stride *stride) 
{

  myStrides.insert(pair<long long, Stride*>(PC, stride));
}

void HistoryStrideTable::addNewStride(long long PC, long long lowAddress, long long highAddress, long long strideLength, int numAccesses, long long accessSize, MemAccessMode readOrWrite)
{
  Stride* temp;
  temp = new Stride(lowAddress, highAddress, strideLength, numAccesses, accessSize, readOrWrite);

  myStrides.insert(pair<long long, Stride*>(PC, temp));  
}


bool HistoryStrideTable::doesStrideExist(long long PC, long long memoryAddr, long long strideLength, MemAccessMode readOrWrite)
{

  pair<multimap<long long, Stride*>::iterator, multimap<long long, Stride*>::iterator> rangeIter;
  multimap<long long, Stride*>::iterator iter;

  rangeIter = myStrides.equal_range(PC);

  for(iter = rangeIter.first; iter != rangeIter.second; iter++){
    if(readOrWrite == iter->second->getAccessMode()){
      if(memoryAddr >= iter->second->getLowAddress() && memoryAddr <= iter->second->getHighAddress()){
	return true;
      }
    }
  }

  return false;
}

bool HistoryStrideTable::updateExistingStride(long long PC, long long memoryAddr, long long strideLength, MemAccessMode readOrWrite)
{

  pair<multimap<long long, Stride*>::iterator, multimap<long long, Stride*>::iterator> rangeIter;
  multimap<long long, Stride*>::iterator iter;

  rangeIter = myStrides.equal_range(PC);

  for(iter = rangeIter.first; iter != rangeIter.second; iter++){
    if(readOrWrite == iter->second->getAccessMode()){
      if(iter->second->addAccess(memoryAddr)  == true){
	return true;
      }
    }
  }

  return false;
}

