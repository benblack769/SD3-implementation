#ifndef STRIDEDETECTOR_H
#define STRIDEDETECTOR_H

// Unknown is when you have observed first address only.
enum MemAccessType { UNDEFINED=-1, UNKNOWN = 0, POINT = 1, STRIDE = 2};

// States of Stride Detector FSM 
enum FSMState {Start = 0, FirstObserved = 1, StrideLearned = 2, WeakStride = 3, StrongStride = 4};


// This represents a Finite State Machine used to detect strides for a given PC.  It
// keeps track of the current state, last address being considered for part of a stride, 
// and the stride distance.
class StrideDetector
{
  
 public: 
  
  // Create new FSM 
  StrideDetector();		
  
  // Returns current FSM state
  FSMState getState() { return myState; }

  // Returns current stride distance
  int getStride() { return myStrideDist; }

  // Return memory address that will serve as base for any stride
  long long getFirstMemAddr() { return myFirstMemAddr; }

  // Return memory address for last access by this PC
  long long getPrevMemAddr() { return myPrevMemAddr; }


  // Given an address, updates the FSM state appropriately.  Returns the type
  // of access (Unknown, Point, Stride) this address was classified as.  
  MemAccessType addAccess(long long address);

 private:

  // Method determines the change in FSM, updating stride distance and address 
  // appropriate.
  // Returns a number representing the change in state.  Negative means
  // the current address didn't fit into the current stride.  Positive means
  // the current address reinforced the current stride.
  int determineStateChange(long long address);

  long long myFirstMemAddr;
  long long myPrevMemAddr; 
  long long myStrideDist;
  
  FSMState myState;
  FSMState myPrevState;

};

#endif
