#ifndef STRIDEDETECTOR_H
#define STRIDEDETECTOR_H

// Unknown is when you have observed first address only.
enum MemAccessType { UNDEFINED=-1, UNKNOWN = 0, LOSTPOINT = 1, NEWPOINT = 2, POINT = 3, STRIDE = 4};

// States of Stride Detector FSM 
enum FSMState {Start = 0, FirstObserved = 1, StrideLearned = 2, WeakStride = 3, StrongStride = 4};


// This represents a Finite State Machine used to detect strides for a given PC.  It
// keeps track of the current state, last address being considered for part of a stride, 
// and the stride distance.
// 
// On the third memory access where the two differences between accesses are
// equal and non-zero, the StrideDetector will classify this as a stride.
//
// In the worst case when in StrongStride and a new stride pattern is emerging, 
// there will be two accesses recognized as Points and two accesses recognized 
// as Unknown.  On the next access, the last 3 points will be included in 
// a stride.  This can clearly be optimized to recognize all 5 points as
// part of the stride, but let's see how important it is to do that before
// putting in the effort.  
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

  // It is only on the third memory access that a stride will be detected.  Before
  // that third access, two memory accesses may have been processed by the stride
  // detector but not had their state as points or strides determined yet.  When
  // either the stride detector has been flushed or the stride detector has returned
  // a LOSTPOINT state, the missing point(s) will be accessible via these two 
  // methods.
  long long getFirstLostPoint() { return myLostPoints[0]; }
  long long getSecondLostPoint() { return myLostPoints[1]; }

  // When tracking finishes, there may still be two memory accesses that have
  // not been put into a stride or point yet.  This method moves them to 
  // be viewed as lost points, so the lost points methods can be used to access
  // those memory references.
  bool flush();
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

  bool myLastAccessUnknown;  // before we get to WeakStride

  // When in StrideLearned, you have seen 2 accesses but won't know if they are a stride until
  // the third access.  If the third access does not have a matching stride, it will be considered
  // the first access of a new potential stride (going back to FirstObserved).  We want to make sure
  // we don't lose the two points that occurred.
  // myStateAlreadyProcessed is used to keep track of whether or not information stored
  // in the StrideDetector has already been processed as a Stride.  This occurs when 
  // the state was previously at >= WeakStride and then the state moved backwards.  We
  // want to make sure that the addresses in the StrideDetector are not reprocessed
  // as Points as they have already been processed as part of a Stride
  long long myLostPoints[2];  
  bool myLostPoint;
  bool myStateAlreadyProcessed;  
  void setLostPoints();
  void clearLostPoints();

  FSMState myState;
  FSMState myPrevState;

};

#endif
