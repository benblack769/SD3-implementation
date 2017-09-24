#ifndef __HISTORYSTRIDETABLE_H__
#define __HISTORYSTRIDETABLE_H__

#include "Stride.h"
#include <map>

using namespace std;

// This class stores all of the strides for the current loop iteration.  It is
// organized using PC.  For each entry, there is a killed status.
class HistoryStrideTable {

  public:
    HistoryStrideTable(){};

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

  private:
    std::multimap<long long, Stride *> myStrides; // (PC, Stride*)
};

#endif
