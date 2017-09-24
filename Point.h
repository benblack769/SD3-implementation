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
    Point(long long memAddress, long long PC, long long numAccesses = 1,
          long long accessSize = 4, MemAccessMode readOrWrite = READ);

    // Return the PC associated with this memory access
    long long getPC() { return myPC; };

    // Return this point's memory address
    long long getMemAddress() { return myMemAddress; };

    // Return the number of times this memory address has been accessed by this
    // PC
    long long getNumAccesses() { return myNumAccesses; };

    // Return the size of the memory access
    long long getAccessSize() { return myAccessSize; };

    // Return the access mode (read or  write)
    MemAccessMode getAccessMode() { return myAccessMode; };

    // Increment the number of accesses
    void addAccess() { myNumAccesses++; };

    void print();

  private:
    long long myPC;
    long long myMemAddress;
    long long myNumAccesses;
    long long myAccessSize;
    MemAccessMode myAccessMode; // read or write
};

#endif
