#include "PendingStrideTable.h"

typedef typename multimap<int64_t, Stride *>::iterator stride_map_iterator;

// This method assumes that a stride does not already exist
void PendingStrideTable::addNewStride(int64_t PC, Stride *stride) {
    myPendingStrides.insert(pair<int64_t, Stride *>(PC, stride));
}

void PendingStrideTable::addNewStride(int64_t PC, int64_t lowAddress, int64_t highAddress, int64_t strideLength,
                                      int numAccesses, int64_t accessSize, MemAccessMode readOrWrite) {
    Stride * stride = new Stride(lowAddress, highAddress, strideLength, numAccesses, accessSize, readOrWrite);
    addNewStride(PC, stride);
}

bool PendingStrideTable::doesStrideExist(int64_t PC, int64_t memoryAddr, int64_t strideLength,
                                         MemAccessMode readOrWrite) {
    pair<stride_map_iterator, stride_map_iterator> rangeIter;
    stride_map_iterator iter;

    rangeIter = myPendingStrides.equal_range(PC);

    for (iter = rangeIter.first; iter != rangeIter.second; iter++) {
        //    cout << "ITER: " << iter->first << endl;
        //    cout << "ITER: " << iter->second.killed << endl;
        //    cout << "ITER: " ;
        //    iter->second.stride->print();
        //    cout << endl;
        if (readOrWrite == iter->second.stride->getAccessMode() && strideLength == iter->second.stride->getStride()) {
            //      cout << "MEMORYADDR " << memoryAddr << " low " <<
            //      iter->second.stride->getLowAddress() << " high " <<
            //      (iter->second.stride->getHighAddress()) << " stride " <<
            //      strideLength << " sum" <<
            //      (iter->second.stride->getHighAddress() + strideLength) <<
            //      endl;
            if (memoryAddr == (iter->second.stride->getHighAddress() + strideLength)) {
                return true;
            }
        }
    }

    return false;
}

bool PendingStrideTable::updateExistingStride(int64_t PC, int64_t memoryAddr, int64_t strideLength,
                                              MemAccessMode readOrWrite) {
    pair<stride_map_iterator, stride_map_iterator> rangeIter;
    stride_map_iterator iter;

    rangeIter = myPendingStrides.equal_range(PC);

    for (iter = rangeIter.first; iter != rangeIter.second; iter++) {
        if (readOrWrite == iter->second.stride->getAccessMode()) {
            if (iter->second.stride->addAccess(memoryAddr) == true) {
                return true;
            }
        }
    }

    return false;
}

bool PendingStrideTable::killStride(int64_t PC, int64_t memoryAddr, int64_t strideLength, MemAccessMode readOrWrite) {
    pair<stride_map_iterator, stride_map_iterator> rangeIter;
    stride_map_iterator iter;

    rangeIter = myPendingStrides.equal_range(PC);

    for (iter = rangeIter.first; iter != rangeIter.second; iter++) {
        if (readOrWrite == iter->second.stride->getAccessMode()) {
            if (memoryAddr >= iter->second.stride->getLowAddress() &&
                memoryAddr <= iter->second.stride->getHighAddress()) {
                iter->second.killed = true;
                return true;
            }
        }
    }

    return false;
}

void PendingStrideTable::print() {
    stride_map_iterator iter;

    for (iter = myPendingStrides.begin(); iter != myPendingStrides.end(); iter++) {
        cout << "PC: " << iter->first;
        cout << dec << " killed: " << iter->second.killed << " ";
        iter->second.stride->print();
        cout << "*****" << endl;
    }
}
