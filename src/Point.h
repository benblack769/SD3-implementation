#ifndef POINT_H
#define POINT_H

#include "Types.h"
#include "PC_ID.h"
#include <iostream>
#include <string>

using namespace std;

class Point:
    public Block{
  public:
    // Create a new point including its memory address and the PC that made the
    // memory access
    Point(int64_t memAddress, int64_t PC, int64_t accessSize = 4,
          MemAccessMode readOrWrite = READ);

    // Return the PC associated with this memory access
    PC_ID getPC_ID() { return myPC; };

    // Return this point's memory address
    int64_t getMemAddress() { return first(); };

    // Return the size of the memory access
    int64_t getAccessSize() { return this->length(); };

    void set_killed(bool value){killed = value;}

    void print();

  private:
    bool killed;
    PC_ID myPC;
};

#endif
