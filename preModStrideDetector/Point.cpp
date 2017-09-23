#include "Point.h"	
#include <iostream>
#include <string>


// Create a new point including its memory address and the PC that made the memory access
Point::Point(long long memAddress, long long PC, long long numAccesses, long long accessSize, MemAccessMode readOrWrite)
{
  myPC = PC;
  myMemAddress = memAddress;
  myNumAccesses = numAccesses;
  myAccessSize = accessSize;
  myAccessMode = readOrWrite;
} 

