#pragma once
#include "Types.h"
#include "IntervalTypes.h"
#include "PC_ID.h"

/*
  We want to output two kinds of data as dependency information:
  
  Instruction level data (for a single loop iteration)
    * Conflicting instructions
    * Approxamate memory access location of the dependence
  
  Loop instance level summary containing
    * Number of loop iterations
    * The number of iterations of the loop that had RAW dependencies
    * A stat (average?) number of memory accesses which had RAW dependencies in a loop iteration
  
  Loop level summary (basically a simple summary of the differnet loop instance summaries)
    * Number of loop instances
    * Stat (average?) number of loop iterations
    * Stat (average?) number of loop iterations with RAW dependencies
    * Stat (average?) number of conflicting memory accesses in a single iteration for those that did have dependencies
*/

enum DependenceType { NODEP = -1, RAW = 0, WAR = 1, WAW = 2 };

class Dependence {
public:
    Dependence(){
        myDependenceType = NODEP;
    }

    Dependence(PC_ID earlier, PC_ID later,int64_t mem_addr,int64_t iteration,int64_t instance){
        loop_instance = instance;
        loop_iteration = iteration;
        
        earlier_instr = earlier;
        later_instr = later;

        myMemoryAddr = mem_addr;

        //sets dependence mode based on reads and write pattern
        if (later_instr.get_acc_mode() == READ) {
            if (earlier_instr.get_acc_mode() == WRITE) {
                myDependenceType = RAW;
            }
        } else {
            if (earlier_instr.get_acc_mode() == READ) {
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
    friend std::ostream &operator<<(std::ostream &os, const Dependence &obj);
  private:
    PC_ID earlier_instr;
    PC_ID later_instr;
    DependenceType myDependenceType;
    int64_t myMemoryAddr;
    int64_t loop_iteration;
    int64_t loop_instance;
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
          ", Later: " << obj.getLater() << 
          ", LoopIteration: " << obj.loop_iteration << 
          ", LoopInstance: " << obj.loop_instance;
    return os;
}
