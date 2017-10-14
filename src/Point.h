#ifndef POINT_H
#define POINT_H

#include "Types.h"
#include <iostream>
#include <string>

using namespace std;

class Point {
  public:
    // Create a new point including its memory address and the PC that made the
    // memory access
    Point(int64_t memAddress, int64_t PC, int64_t numAccesses = 1, int64_t accessSize = 4,
          MemAccessMode readOrWrite = READ);

    // Return the PC associated with this memory access
    int64_t getPC() { return myPC; };

    // Return this point's memory address
    int64_t getMemAddress() { return myMemAddress; };

    // Return the number of times this memory address has been accessed by this
    // PC
    int64_t getNumAccesses() { return myNumAccesses; };

    // Return the size of the memory access
    int64_t getAccessSize() { return myAccessSize; };

    // Return the access mode (read or  write)
    MemAccessMode getAccessMode() { return myAccessMode; };

    // Increment the number of accesses
    void addAccess() { myNumAccesses++; };

    void print();

  private:
    int64_t myPC;
    int64_t myMemAddress;
    int64_t myNumAccesses;
    int64_t myAccessSize;
    MemAccessMode myAccessMode; // read or write
};

#endif
