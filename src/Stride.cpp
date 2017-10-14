#include "Stride.h"
#include <iostream>
#include <stdlib.h>
#include <string>

// The assumption is that the stride distance is the difference between the two
// addresses and the number of accesses is by default 2.
Stride::Stride(int64_t lowAddress, int64_t highAddress, int64_t accessSize, MemAccessMode readOrWrite) {
    myLowAddress = lowAddress;
    myHighAddress = highAddress;
    myStrideLength = (highAddress - lowAddress);
    myNumAccesses = 2;
    myAccessMode = readOrWrite;
    myAccessSize = accessSize;
}

Stride::Stride(int64_t lowAddress, int64_t highAddress, int strideLength, int numAccesses, int64_t accessSize,
               MemAccessMode readOrWrite) {
    myLowAddress = lowAddress;
    myHighAddress = highAddress;
    myStrideLength = strideLength;
    myNumAccesses = numAccesses;
    myAccessMode = readOrWrite;
    myAccessSize = accessSize;
}

// This method will update the low or high address as appropriate.  However, it
// will only do so if the distance beween the address and that low or high is
// the stride length
bool Stride::addAccess(int64_t address) {
    int64_t diff = address - myHighAddress;

    if (diff != myStrideLength) {
        return false;
    } else {
        myHighAddress = address;
        myNumAccesses++;
        return true;
    }
}

// This method returns a boolean value indicating whether or not the specified
// address has actually been accessed as part of this stride.
bool Stride::hasBeenAccessed(int64_t address) {
    if (address < myLowAddress || address > myHighAddress) {
        return false;
    }

    int stride = abs(myStrideLength);
    int64_t currentAddress = myLowAddress;

    while (currentAddress <= myHighAddress) {
        if (currentAddress == address) {
            return true;
        }
        currentAddress += stride;
    }
    return false;
}

void Stride::print() {
    cout << "[" << myLowAddress << "," << myHighAddress << "] Stride: " << dec << myStrideLength;
    cout << " #Accesses: " << myNumAccesses << " Size: " << myAccessSize;
    cout << " Mode: " << myAccessMode;
    cout << endl;
}
