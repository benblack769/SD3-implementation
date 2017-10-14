#include "HistoryStrideTable.h"

// This method assumes that a stride does not already exist
void HistoryStrideTable::addNewStride(int64_t PC, Stride *stride) {
    myStrides.insert(pair<int64_t, Stride *>(PC, stride));
}

void HistoryStrideTable::addNewStride(int64_t PC, int64_t lowAddress, int64_t highAddress, int64_t strideLength,
                                      int numAccesses, int64_t accessSize, MemAccessMode readOrWrite) {
    Stride *temp;
    temp = new Stride(lowAddress, highAddress, strideLength, numAccesses, accessSize, readOrWrite);

    myStrides.insert(pair<int64_t, Stride *>(PC, temp));
}

bool HistoryStrideTable::doesStrideExist(int64_t PC, int64_t memoryAddr, int64_t strideLength,
                                         MemAccessMode readOrWrite) {
    pair<multimap<int64_t, Stride *>::iterator, multimap<int64_t, Stride *>::iterator> rangeIter;
    multimap<int64_t, Stride *>::iterator iter;

    rangeIter = myStrides.equal_range(PC);

    for (iter = rangeIter.first; iter != rangeIter.second; iter++) {
        if (readOrWrite == iter->second->getAccessMode()) {
            if (memoryAddr >= iter->second->getLowAddress() && memoryAddr <= iter->second->getHighAddress()) {
                return true;
            }
        }
    }

    return false;
}

bool HistoryStrideTable::updateExistingStride(int64_t PC, int64_t memoryAddr, int64_t strideLength,
                                              MemAccessMode readOrWrite) {
    pair<multimap<int64_t, Stride *>::iterator, multimap<int64_t, Stride *>::iterator> rangeIter;
    multimap<int64_t, Stride *>::iterator iter;

    rangeIter = myStrides.equal_range(PC);

    for (iter = rangeIter.first; iter != rangeIter.second; iter++) {
        if (readOrWrite == iter->second->getAccessMode()) {
            if (iter->second->addAccess(memoryAddr) == true) {
                return true;
            }
        }
    }

    return false;
}
