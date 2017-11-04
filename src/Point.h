#pragma once

#include <iostream>
#include <string>
#include "Types.h"
#include "PC_ID.h"
#include "dynamic_gcd.h"

using namespace std;

class KilledSpot{
public:
    KilledSpot():
        killed(false){}
    void set_killed(bool value){killed = value;}
    bool is_killed(){return killed;}
protected:
    bool killed;
};
class PC_info{
public:
    PC_info(PC_ID in_pc):
        myPC(in_pc){}
    PC_info():
        myPC(null_id){}
    PC_ID getPC_ID() { return myPC; }
protected:
    PC_ID myPC;
};

class Point:
    public Block,
    public KilledSpot,
    public PC_info{
  public:
    // Create a new point including its memory address and the PC that made the
    // memory access
    Point(int64_t memAddress, int64_t accessSize, int64_t PC, MemAccessMode readOrWrite):
        Block(memAddress,memAddress+accessSize),
        PC_info(PC_ID(PC,readOrWrite)){}
    Point(Block in_block, PC_ID access_id):
        Block(in_block),
        PC_info(access_id){}
    Point(){}
};
class Stride:
    public SparseStride,
    public PC_info,
    public KilledSpot{
  public:
    // Create a new point including its memory address and the PC that made the
    // memory access
    Stride(int64_t lowAddress, int64_t num_strides, int64_t strideLength, int64_t accessSize, int64_t PC,  MemAccessMode readOrWrite):
        SparseStride(lowAddress,num_strides,strideLength,accessSize),
        PC_info(PC_ID(PC,readOrWrite)){}
    
    Stride(SparseStride in_stride, PC_ID access_id):
        SparseStride(in_stride),
        PC_info(access_id){}
    Stride(){}
};
