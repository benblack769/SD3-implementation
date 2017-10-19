#include "Point.h"
#include <iostream>
#include <string>

// Create a new point including its memory address and the PC that made the
// memory access
Point::Point(int64_t memAddress, int64_t PC, int64_t numAccesses, int64_t accessSize, MemAccessMode readOrWrite) {
    myPC = PC;
    myMemAddress = memAddress;
    myNumAccesses = numAccesses;
    myAccessSize = accessSize;
    myAccessMode = readOrWrite;
}

void Point::print() {
    cout << "PC: " << myPC;
    cout << " Address: " << myMemAddress;
    cout << " #Accesses: " << myNumAccesses << " Size: " << myAccessSize;
    cout << " Mode: " << myAccessMode;
    cout << endl;
}
