#ifndef PENDINGSTRIDETABLE_H
#define PENDINGSTRIDETABLE_H

#include "Stride.h"
#include <iostream>
#include <map>

using namespace std;

// This class stores all of the strides for the current loop iteration.  It is
// organized using PC.  For each entry, there is a killed status.
class PendingStrideTable {

  public:
    PendingStrideTable(){};

    void addNewStride(long long PC, Stride *stride);

    void addNewStride(long long PCAddress, long long lowAddress,
                      long long highAddress, long long strideLength,
                      int numAccesses = 2, long long accessSize = 4,
                      MemAccessMode readOrWrite = READ);

    bool doesStrideExist(long long PCAddress, long long memoryAddr,
                         long long strideLength,
                         MemAccessMode readOrWrite = READ);

    bool updateExistingStride(long long PCAddress, long long memoryAddr,
                              long long strideLength,
                              MemAccessMode readOrWrite = READ);

    bool killStride(long long PCAddress, long long memoryAddr,
                    long long strideLength, MemAccessMode readOrWrite);

    void print();

  private:
    struct StrideEntry {
        Stride *stride;
        bool killed;

        StrideEntry() {
            stride = NULL;
            killed = false;
        };
    };

    std::multimap<long long, StrideEntry> myPendingStrides; // (PC, Stride*)
};

#endif
