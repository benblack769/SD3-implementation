#ifndef PENDINGSTRIDETABLE_H
#define PENDINGSTRIDETABLE_H

#include "Stride.h"
#include "Types.h"
#include <iostream>
#include <map>

using namespace std;

// This class stores all of the strides for the current loop iteration.  It is
// organized using PC.  For each entry, there is a killed status.
class PendingStrideTable {

public:
    PendingStrideTable(){};

    void addNewStride(int64_t PC, Stride * stride);

    void addNewStride(int64_t PCAddress, int64_t lowAddress, int64_t highAddress, int64_t strideLength,
                      int numAccesses = 2, int64_t accessSize = 4, MemAccessMode readOrWrite = READ);

    //bool doesStrideExist(int64_t PCAddress, int64_t memoryAddr, int64_t strideLength, MemAccessMode readOrWrite = READ);

    //bool updateExistingStride(int64_t PCAddress, int64_t memoryAddr, int64_t strideLength,
    //                          MemAccessMode readOrWrite = READ);

    //bool killStride(int64_t PCAddress, int64_t memoryAddr, int64_t strideLength, MemAccessMode readOrWrite);

    void print();

protected:
    std::multimap<int64_t, Stride *> myPendingStrides; // (PC, Stride*)
};

#endif
