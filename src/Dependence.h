#pragma once
#include "Types.h"
#include "IntervalTypes.h"
#include "PC_ID.h"

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
    DependenceType getDependenceType() const { return myDependenceType; }

    int64_t getApproxamateMemoryAddress() const { return myMemoryAddr; }
    
    PC_ID getEarlier()const {return earlier_instr;}
    PC_ID getLater()const {return later_instr;}
  private:
    PC_ID earlier_instr;
    PC_ID later_instr;
    DependenceType myDependenceType;
    int64_t myMemoryAddr;
};

inline const char * DependenceTypeString(DependenceType type){
    switch(type){
    case NODEP:return "NODEP";
    case RAW:return "RAW";
    case WAR:return "WAR";
    case WAW:return "WAW";
    }
    assert(false);
}

inline std::ostream &operator<<(std::ostream &os, const Dependence &obj) {
    os << "Dependence: " << DependenceTypeString(obj.getDependenceType()) << 
          ", AproxMemAddr: " << obj.getApproxamateMemoryAddress() << 
          ", Earlier: " << obj.getEarlier() <<  
          ", Later: " << obj.getLater();
    return os;
}
