#include "StrideDetector.h"
#include <iostream>

// Creates a new FSM 
StrideDetector::StrideDetector()
{
  myPrevMemAddr = 0; 
  myFirstMemAddr = 0;
  myStrideDist = 0;
  
  myState = Start;
  myPrevState = Start;
}

// Given an address, updates the FSM state appropriately.  Returns the type
// of access (Unknown, POINT, STRIDE) this address was classified as.  
MemAccessType StrideDetector::addAccess(long long address)
{
  // changes state and returned value indicates forward or backward motion in FSM
  int change = determineStateChange(address);

  std::cout <<"\t ADDR " << address << " change " << change << " distance " << myStrideDist << std::endl;

  // This is the first access, so we don't know if it is a Point or a Stride
  if(myPrevState == Start){
    return UNKNOWN;
  }

  if(change < 0){
    // moved backwards in FSM 

    if(myState == FirstObserved){
      // This doesn't match old stride but it could be start of new stride
      return UNKNOWN;
    }
    else{
      // This doesn't match old stride but it could be an outlier.  There will be
      // several outliers before we recognize a new stride.
      return POINT;
    }
  }
  else{
    // access reinforced existing stride length

    if(myStrideDist == 0){
      // fixed address
      return POINT;
    }
    else{
      return STRIDE;
    }
  }

}

// Method determines the change in FSM, updating stride distance and address 
// appropriate.
// Returns a number representing the change in state.  Negative means
// the current address didn't fit into the current stride.  Positive means
// the current address reinforced the current stride.
int StrideDetector::determineStateChange(long long address)
{
  int diff = address - myPrevMemAddr;
  
  if (myState == Start){ // Start //
      myFirstMemAddr = address;
      
      myPrevState = myState;
      myState = FirstObserved;
  }
  else if (myState == FirstObserved){ // First Observed //
    diff = address - myFirstMemAddr;  // special case
    myStrideDist = diff;
    
    myPrevMemAddr = address;
    myPrevState = myState;
    myState = StrideLearned;
  }
  else if (myState == StrideLearned){ // Stride Learned State //
    
    if(diff == myStrideDist){
      // Not a fixed address, so repeated stride
      if(diff != 0){
	myPrevMemAddr = address;
	
	myPrevState = myState;
	myState = WeakStride;
      }
      else{
	// fixed address
      }
    }
    else{  // Old stride is now gone
      myFirstMemAddr = address;
      myPrevMemAddr = 0;
      myStrideDist = 0;

      myPrevState = myState;
      myState = FirstObserved;
    }
    
  }
  else if (myState == WeakStride){ // Weak Stride State //
    if(diff == myStrideDist){
	myPrevMemAddr = address;
	
	myPrevState = myState;
	myState = StrongStride;
    }
    else{
      //current address doesn't match current stride
      myPrevState = myState;
      myState = StrideLearned;
    }
  }
  else if (myState == StrongStride){ // Strong Stride State //
    if(diff == myStrideDist){
	myPrevMemAddr = address;

	myPrevState = myState;
	myState = StrongStride;
    }
    else{
      // current address doesn't match current stride
      myPrevState = myState;
      myState = WeakStride;
    }
  }

  int stateChange = myState - myPrevState;
  return stateChange;
}






