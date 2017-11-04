#include "StrideDetector.h"
#include <iostream>

// Creates a new FSM
StrideDetector::StrideDetector() {
    myPrevMemAddr = 0;
    myFirstMemAddr = 0;
    myStrideDist = 0;

    myState = Start;
}

// Given an address, updates the FSM state appropriately.  Returns the type
// of access (Unknown, POINT, STRIDE) this address was classified as.
MemAccessType StrideDetector::addAccess(int64_t address) {
    // changes state and returned value indicates forward or backward motion in
    // FSM
    int change = determineStateChange(address);

    std::cout << "\t ADDR " << address << " change " << change << " distance " << myStrideDist << std::endl;

    switch(myState){
    case Start: return POINT;
    case FirstObserved: return POINT;
    case StrideLearned: return POINT;
    case WeakStride: return STRIDE;
    case StrongStride: return STRIDE;
    }
}

// Method determines the change in FSM, updating stride distance and address
// appropriate.
// Returns a number representing the change in state.  Negative means
// the current address didn't fit into the current stride.  Positive means
// the current address reinforced the current stride.
//
// As of right now, this is written with strict increasing or decreasing.
// The SD3 paper considers all of these to be part of a stride
// [10, 14, 18, 14, 18, 22, 18, 22, 26]
// For now, I'm not counting this as a single stride, which is different from
// SD3.  I'm going to go with strictly increasing or decreasing and then
// see what difference it makes to have this strict definition of a stride
// in terms of memory compression.
int StrideDetector::determineStateChange(int64_t address) {
    int diff = address - myPrevMemAddr;

    FSMState myPrevState = myState;
    if (myState == Start) { // Start //
        myFirstMemAddr = address;

        myState = FirstObserved;
    } else if (myState == FirstObserved) { // First Observed //
        diff = address - myFirstMemAddr;   // special case
        myStrideDist = diff;

        myPrevMemAddr = address;
        myState = StrideLearned;
    } else if (myState == StrideLearned) { // Stride Learned State //

        if (diff == myStrideDist) {
            // Not a fixed address, so repeated stride
            if (diff != 0) {
                myPrevMemAddr = address;

                myState = WeakStride;
            } 
        } else { // Old stride is now gone
            myFirstMemAddr = address;
            myPrevMemAddr = 0;
            myStrideDist = 0;

            myState = FirstObserved;
        }

    } else if (myState == WeakStride) { // Weak Stride State //
        if (diff == myStrideDist) {
            myPrevMemAddr = address;

            myState = StrongStride;
        } else {
            // current address doesn't match current stride
            myState = StrideLearned;
        }
    } else if (myState == StrongStride) { // Strong Stride State //
        if (diff == myStrideDist) {
            myPrevMemAddr = address;

            myState = StrongStride;
        } else {
            // current address doesn't match current stride
            myState = WeakStride;
        }
    }

    int stateChange = myState - myPrevState;
    return stateChange;
}
