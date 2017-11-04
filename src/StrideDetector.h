#ifndef STRIDEDETECTOR_H
#define STRIDEDETECTOR_H

#include "Types.h"
// Unknown is when you have observed first address only.
enum MemAccessType { UNDEFINED = -1, POINT = 0, STRIDE = 1};

// States of Stride Detector FSM
enum FSMState { Start = 0, FirstObserved = 1, StrideLearned = 2, WeakStride = 3, StrongStride = 4 };

// This represents a Finite State Machine used to detect strides for a given PC.
// It keeps track of the current state, last address being considered for part
// of a stride, and the stride distance.
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
class StrideDetector {

  public:
    // Create new FSM
    StrideDetector();

    // Returns current FSM state
    FSMState getState() { return myState; }

    // Returns current stride distance
    int getStride() { return myStrideDist; }

    // Given an address, updates the FSM state appropriately.  Returns the type
    // of access (Unknown, Point, Stride) this address was classified as.
    MemAccessType addAccess(int64_t address);

  private:
    // Method determines the change in FSM, updating stride distance and address
    // appropriate.
    // Returns a number representing the change in state.  Negative means
    // the current address didn't fit into the current stride.  Positive means
    // the current address reinforced the current stride.
    int determineStateChange(int64_t address);

    int64_t myFirstMemAddr;
    int64_t myPrevMemAddr;
    int64_t myStrideDist;
    
    FSMState myState;
};

#endif
