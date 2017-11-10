#pragma once
#include "Types.h"
#include "IntervalTypes.h"

enum DependenceType { NODEP = -1, RAW = 0, WAR = 1, WAW = 2 };

class Dependence {
public:
    Dependence(){
        myDependenceType = NODEP;
    }

    Dependence(PC_ID earlier, PC_ID later,int64_t mem_addr){
        earlier_instr = earlier;
        later_instr = later;

        mem_addr = myMemoryAddr;

        //sets deppendence mode based on reads and write pattern
        if (earlier_instr.get_acc_mode() == READ) {
            if (later_instr.get_acc_mode() == WRITE) {
                myDependenceType = RAW;
            }
        } else {
            if (later_instr.get_acc_mode() == READ) {
                myDependenceType = WAR;
            } else {
                myDependenceType = WAW;
            }
        }
    }

    DependenceType getDependenceType() { return myDependenceType; }

    int64_t getMemoryAddress() { return myMemoryAddr; }

  private:
    PC_ID earlier_instr;
    PC_ID later_instr;
    DependenceType myDependenceType;
    int64_t myMemoryAddr;
};
