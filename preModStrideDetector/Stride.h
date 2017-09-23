#ifndef STRIDE_H
#define STRIDE_H

#include <string>
#include "Types.h"

// This class stores relevent information about a memory address that has been determined to be part
// of a stride.  
class Stride
{
  
 public:
  // The assumption is that the stride distance is the difference between the two addresses and the
  // number of accesses is by default 2.  
  Stride(long long lowAddress, long long highAddress, long long accessSize=4, MemAccessMode readOrWrite=READ);

  Stride(long long lowAddress, long long highAddress, int strideLength, int numAccesses, long long accessSize=4, 
	 MemAccessMode readOrWrite=READ);

  // This method will update the low or high address as appropriate.  However, it will only do so
  // if the distance beween the address and that low or high is the stride length
  bool addAccess(long long address);

  // Returns the lowest address
  long long getLowAddress() { return myLowAddress; };

  // Return the highest address
  long long getHighAddress() { return myHighAddress; };

  // Return the length of the stride
  int getStride() { return myStrideLength; };

  // Return the number of accesses
  int getNumAccesses() { return myNumAccesses; };

  // Return the access mode (read or write)
  MemAccessMode getAccessMode() { return myAccessMode; };

  // This method returns a boolean value indicating whether or not the specified
  // address has actually been accessed as part of this stride.
  bool hasBeenAccessed(long long address);

 private:
  long long myLowAddress;
  long long myHighAddress;
  int myStrideLength;
  long long myAccessSize;
  long long myNumAccesses; 
  MemAccessMode myAccessMode;  // read or write
};

#endif
