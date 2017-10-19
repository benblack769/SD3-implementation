#ifndef __HISTORYSTRIDETABLE_H__
#define __HISTORYSTRIDETABLE_H__

#include "Stride.h"
#include "Types.h"
#include <map>

using namespace std;

// This class stores all of the strides for the current loop iteration.  It is
// organized using PC.  For each entry, there is a killed status.
class HistoryStrideTable {

  public:
    HistoryStrideTable(){};

    void addNewStride(int64_t PC, Stride *stride);

    void addNewStride(int64_t PCAddress, int64_t lowAddress, int64_t highAddress, int64_t strideLength,
                      int numAccesses = 2, int64_t accessSize = 4, MemAccessMode readOrWrite = READ);

    bool doesStrideExist(int64_t PCAddress, int64_t memoryAddr, int64_t strideLength, MemAccessMode readOrWrite = READ);

    bool updateExistingStride(int64_t PCAddress, int64_t memoryAddr, int64_t strideLength,
                              MemAccessMode readOrWrite = READ);

  private:
    std::multimap<int64_t, Stride *> myStrides; // (PC, Stride*)
};

#endif
