#ifndef STRIDE_H
#define STRIDE_H

#include <iostream>
#include <string>
#include "Types.h"
#include "PC_ID.h"
#include "dynamic_gcd.h"

using namespace std;

// This class stores relevent information about a memory address that has been
// determined to be part of a stride.
class Stride {

  public:
    // The assumption is that the stride distance is the difference between the
    // two addresses and the number of accesses is by default 2.
    Stride(int64_t lowAddress, int64_t highAddress, int64_t accessSize = 4, MemAccessMode readOrWrite = READ);

    Stride(int64_t lowAddress, int64_t highAddress, int strideLength, int numAccesses, int64_t accessSize = 4,
           MemAccessMode readOrWrite = READ);

  private:
      SparseStride stride;
      PC_ID pc;
      bool killed;
};

#endif
