#include "StrideDetector.h"
#include <iostream>

// Creates a new FSM
StrideDetector::StrideDetector() {
    myPrevMemAddr = 0;
    myFirstMemAddr = 0;
    myStrideDist = 0;

    myLastAccessUnknown = true;
    myLostPoints[0] = myLostPoints[1] = 0;
    myLostPoint = false;
    myStateAlreadyProcessed = false;

    myState = Start;
    myPrevState = Start;
}

// Given an address, updates the FSM state appropriately.  Returns the type
// of access (Unknown, POINT, STRIDE) this address was classified as.
MemAccessType StrideDetector::addAccess(long long address) {
    if (myLostPoint == true) {
        // We assume that the lost points were handled before the next access
        // was processed.
        clearLostPoints();
    }

    // changes state and returned value indicates forward or backward motion in
    // FSM
    int change = determineStateChange(address);

    std::cout << "\t ADDR " << address << " change " << change << " distance " << myStrideDist << std::endl;

    // This is the first access, so we don't know if it is a Point or a Stride
    if (myPrevState == Start) {
        myStateAlreadyProcessed = false;
        return UNKNOWN;
    }

    if (change < 0) {
        // moved backwards in FSM

        if (myState == FirstObserved) {
            if (myLostPoint == true) {
                // This occurs when we had two memory accesses but the third
                // memory access didn't match the stride of the first two
                // accesses.  So, we're backing up, but we want to make sure
                // that we don't lose those two preceding accesses that were
                // never previously viewed as part of a stride.
                return LOSTPOINT;
            } else {
                // This doesn't match old stride but it could be start of new
                // stride The only way for this to occur is if you were backing
                // up from StrideLearned and this had previously been recognized
                // as a stride
                return UNKNOWN;
            }
        } else {
            // This doesn't match old stride but it could be an outlier.  There
            // will be several outliers before we recognize a new stride.
            return POINT;
        }
    } else {
        // access reinforced existing stride length

        if (myState == StrideLearned) {
            myStateAlreadyProcessed = false;
            if (myStrideDist == 0 && myPrevState == StrideLearned) {
                // fixed address
                if (myLastAccessUnknown == true) {
                    // Need to recognize there were 3 accesses to this address
                    // that haven't been tracked yet.
                    myLastAccessUnknown = false; // clear boolean so next access
                                                 // is counted singularly
                    return NEWPOINT;
                } else {
                    return POINT;
                }
            }

            return UNKNOWN; // Only on 3rd memory access do we classify that
                            // third access as part of a stride
        } else {
            myStateAlreadyProcessed = true;
            return STRIDE;
        }
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
int StrideDetector::determineStateChange(long long address) {
    int diff = address - myPrevMemAddr;

    if (myState == Start) { // Start //
        myFirstMemAddr = address;

        myPrevState = myState;
        myState = FirstObserved;
        myLastAccessUnknown = true;
    } else if (myState == FirstObserved) { // First Observed //
        diff = address - myFirstMemAddr;   // special case
        myStrideDist = diff;

        myPrevMemAddr = address;
        myPrevState = myState;
        myState = StrideLearned;
        myLastAccessUnknown = true;
    } else if (myState == StrideLearned) { // Stride Learned State //

        if (diff == myStrideDist) {
            // Not a fixed address, so repeated stride
            if (diff != 0) {
                myPrevMemAddr = address;

                myPrevState = myState;
                myState = WeakStride;
            } else {
                // fixed address
                // new state and old state are both StrideLearned
                myPrevState = StrideLearned;
            }
        } else { // Old stride is now gone
            if (myPrevState == FirstObserved) {
                // This is a lost point scenario
                setLostPoints();
            }

            myFirstMemAddr = address;
            myPrevMemAddr = 0;
            myStrideDist = 0;

            myPrevState = myState;
            myState = FirstObserved;
        }

    } else if (myState == WeakStride) { // Weak Stride State //
        if (diff == myStrideDist) {
            myPrevMemAddr = address;

            myPrevState = myState;
            myState = StrongStride;
        } else {
            // current address doesn't match current stride
            myPrevState = myState;
            myState = StrideLearned;
        }
    } else if (myState == StrongStride) { // Strong Stride State //
        if (diff == myStrideDist) {
            myPrevMemAddr = address;

            myPrevState = myState;
            myState = StrongStride;
        } else {
            // current address doesn't match current stride
            myPrevState = myState;
            myState = WeakStride;
        }
    }

    int stateChange = myState - myPrevState;
    return stateChange;
}

void StrideDetector::setLostPoints() {
    if (!myStateAlreadyProcessed) {
        myLostPoints[0] = myFirstMemAddr;
        myLostPoints[1] = myPrevMemAddr;
        myLostPoint = true;
    }
}

void StrideDetector::clearLostPoints() {
    myLostPoints[0] = 0;
    myLostPoints[1] = 0;
    myLostPoint = false;
    myStateAlreadyProcessed = false;
}

bool StrideDetector::flush() {
    // if(myState < WeakStride){
    if (myState < WeakStride && !myStateAlreadyProcessed) {
        setLostPoints();
        return true;
    } else {
        return false;
    }
}
