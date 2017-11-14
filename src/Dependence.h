#pragma once
#include "Types.h"
#include "IntervalTypes.h"
#include "PC_ID.h"
#include <vector>

enum DependenceType { NODEP = -1, RAW = 0, WAR = 1, WAW = 2 };

class InstrDependence {
    /*
   Instruction level data (for a single loop iteration)
    * Conflicting instructions
    * Approxamate memory access location of the dependence
    * Number of memory access conflicts
   */
public:
    InstrDependence();

    InstrDependence(PC_ID earlier, PC_ID later, int64_t mem_addr, int64_t num_conflicts, int64_t num_conflict_iterations=1);
    bool can_summarize(const InstrDependence & other);

    void sum_num_conflicts(const InstrDependence & other);

    DependenceType getDependenceType() const { return myDependenceType; }
    
    int64_t getApproxamateMemoryAddress() const { return myMemoryAddr; }
    int64_t getNumConflicts() const { return numConflicts; }
    
    PC_ID getEarlier()const {return earlier_instr;}
    PC_ID getLater()const {return later_instr;}
    friend std::ostream &operator<<(std::ostream &os, const InstrDependence &obj);
  private:
    PC_ID earlier_instr;
    PC_ID later_instr;
    DependenceType myDependenceType;
    int64_t myMemoryAddr;
    int64_t numConflicts;
    int64_t num_conflict_iters;
};
std::ostream &operator<<(std::ostream &os, const InstrDependence &obj);

class InstructionSetSummary{
public:
    void instr_summarize(const vector<InstrDependence> & instr_deps);
    void set_summarize(const InstructionSetSummary & dep_set);
    friend std::ostream &operator<<(std::ostream &os, const InstructionSetSummary &obj) ;
protected:
    vector<InstrDependence> summaries;
    
    bool try_summarize(const InstrDependence & other);
};

std::ostream &operator<<(std::ostream &os, const InstructionSetSummary &obj);


  
class LoopInstanceDep{
    /*
    Loop instance level summary containing
      * Number of loop iterations
      * The number of iterations of the loop that had RAW dependencies
      * A stat (average?) number of memory accesses which had RAW dependencies in a loop iteration
      * Instruction level data(for a loop instance): same type as for a single loop iteration
    */
public:
    LoopInstanceDep();
    void addIterationDependencies(const vector<InstrDependence> & inst_deps_loop_instance,int64_t num_mem_conflicts_in_iter);
    void addIterationDepsNoInstrs(int64_t num_mem_conflicts_in_iter);
    
    friend std::ostream &operator<<(std::ostream &os, const LoopInstanceDep &obj);
    friend class LoopTotalSummary;
protected:
    int64_t loop_iterations;
    int64_t actual_conflict_iterations;
    int64_t total_memory_conflicts;
    InstructionSetSummary instr_summary;
};


std::ostream &operator<<(std::ostream &os, const LoopInstanceDep &obj);

class LoopTotalSummary{
    /*
  Loop level summary (basically a simple summary of the differnet loop instance summaries)
    * Number of loop instances
    * Stat (average?) number of loop iterations
    * Stat (average?) number of loop iterations with RAW dependencies
    * Stat (average?) number of conflicting memory accesses in a single iteration for those that did have dependencies
*/
public:
    LoopTotalSummary();
    void addLoopInstanceSummary(const LoopInstanceDep & add);
    friend std::ostream &operator<<(std::ostream &os, const LoopTotalSummary &obj);
protected:
    int64_t num_instances;
    int64_t total_iterations;
    int64_t total_conflict_iterations;
    int64_t total_mem_conflicts;
    
    InstructionSetSummary instr_summary;
};

std::ostream &operator<<(std::ostream &os, const LoopTotalSummary &obj);
