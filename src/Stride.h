#ifndef STRIDE_H
#define STRIDE_H

#include "Types.h"
#include "PC_ID.h"
#include <iostream>
#include <string>

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

    // This method will update the low or high address as appropriate.  However,
    // it will only do so if the distance beween the address and that low or
    // high is the stride length
    bool addAccess(int64_t address);

    // Returns the lowest address
    int64_t getLowAddress() { return myLowAddress; };

    // Return the highest address
    int64_t getHighAddress() { return myHighAddress; };

    // Return the length of the stride
    int getStride() { return myStrideLength; };

    // Return the number of accesses
    int getNumAccesses() { return myNumAccesses; };

    // Return the access mode (read or write)
    MemAccessMode getAccessMode() { return myAccessMode; };

    // This method returns a boolean value indicating whether or not the
    // specified address has actually been accessed as part of this stride.
    bool hasBeenAccessed(int64_t address);

    void set_killed(bool value){killed = value;}
    bool is_killed(){return killed;}

    void print();

  private:
      SparseStride stride;
      PC_ID pc;
      bool killed;
};

#endif
