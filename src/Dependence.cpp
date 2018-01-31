#include "Dependence.h"

DependenceType get_dep_type(MemAccessMode earlier,MemAccessMode later){
    if (later == READ) {
        if (earlier == WRITE) {
            return RAW;
        }
    } else {
        if (earlier == READ) {
            return WAR;
        } else {
            return WAW;
        }
    }
    assert(false && "got dependence of RAR which is not a dependence");
    return RAW;
}

InstrDependence::InstrDependence(){
    myDependenceType = NODEP;
}

InstrDependence::InstrDependence(PC_ID earlier, PC_ID later, int64_t mem_addr, int64_t num_conflicts, int64_t num_conflict_iterations){
    earlier_instr = earlier;
    later_instr = later;

    myMemoryAddr = mem_addr;
    numConflicts = num_conflicts;

    num_conflict_iters = num_conflict_iterations;

    myDependenceType = get_dep_type(earlier_instr.get_acc_mode(),later_instr.get_acc_mode());
}
bool InstrDependence::can_summarize(const InstrDependence & other){
    return earlier_instr == other.earlier_instr &&
            later_instr == other.later_instr &&
            myDependenceType == other.myDependenceType;
}
void InstrDependence::sum_num_conflicts(const InstrDependence & other){
    assert(can_summarize(other));
    numConflicts += other.numConflicts;
    num_conflict_iters += other.num_conflict_iters;
}

void InstructionSetSummary::instr_summarize(const vector<InstrDependence> & instr_deps){
    for(size_t i = 0; i < instr_deps.size();i++){
        if(!try_summarize(instr_deps[i])){
            summaries.push_back(instr_deps[i]);
        }
    }
}
void InstructionSetSummary::set_summarize(const InstructionSetSummary & dep_set){
    instr_summarize(dep_set.summaries);
}
bool InstructionSetSummary::try_summarize(const InstrDependence & other){
    for(size_t j = 0; j < summaries.size(); j++){
        if(summaries[j].can_summarize(other)){
            summaries[j].sum_num_conflicts(other);
            return true;
        }
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const InstructionSetSummary &obj) {
    os << "[ \n";
    for(size_t i = 0; i < obj.summaries.size(); i++){
        os << obj.summaries[i];
        if(i != obj.summaries.size()-1){
            os << ",\n";
        }
    }
    os << " ]";
    return os;
}


const char * DependenceTypeString(DependenceType type){
    switch(type){
    case NODEP:return "NODEP";
    case RAW:return "RAW";
    case WAR:return "WAR";
    case WAW:return "WAW";
    }
    assert(false && "bad dependence string");
    return "ERROR";
}

std::ostream &operator<<(std::ostream &os, const InstrDependence &obj) {
    os << //"\"" << DependenceTypeString(obj.getDependenceType()) << "\" : " <<
        "{ " <<
          "\"AproxMemAddr\": " << hex <<"\""<<obj.getApproxamateMemoryAddress() << "\""<<", " <<
          "\"NumConflicts\": " << dec << obj.getNumConflicts() << ", " <<
          "\"EarlierPC\": " << hex << "\""<<obj.getEarlier().get_pc() <<"\""<< ", " <<
          "\"LaterPC\": " << hex << "\""<<obj.getLater().get_pc() <<"\""<< ", " <<
          "\"NumConflictIters\": " << dec << obj.num_conflict_iters <<
         " } ";
    return os;
}
LoopInstanceDep::LoopInstanceDep(){
    loop_iterations = 0;
    actual_conflict_iterations = 0;
    total_memory_conflicts = 0;
}
void LoopInstanceDep::addIterationDependencies(const vector<InstrDependence> & inst_deps_loop_instance, int64_t num_mem_conflicts_in_iter){
    instr_summary.instr_summarize(inst_deps_loop_instance);
    addIterationDepsNoInstrs(num_mem_conflicts_in_iter);
}
void LoopInstanceDep::addIterationDepsNoInstrs(int64_t num_mem_conflicts_in_iter){
    loop_iterations++;
    actual_conflict_iterations += num_mem_conflicts_in_iter == 0 ? 0 : 1;
    total_memory_conflicts += num_mem_conflicts_in_iter;
}

std::ostream &operator<<(std::ostream &os, const LoopInstanceDep &obj) {
    os << "{ \"LoopIterations\": " << obj.loop_iterations <<  ", " <<
          "\"ConflictedIterations\": " << obj.actual_conflict_iterations <<  ", " <<
          "\"TotalConflictingBytes\": " << obj.total_memory_conflicts <<  ", " <<
          "\"InstructionSummary\": " << obj.instr_summary <<
          " }";
    os << endl;
    return os;
}

LoopTotalSummary::LoopTotalSummary(){
    num_instances = 0;
    num_conflict_instances = 0;
    total_iterations = 0;
    total_conflict_iterations = 0;
    total_mem_conflicts = 0;
}

void LoopTotalSummary::addLoopInstanceSummary(const LoopInstanceDep & add){
    num_instances += 1;
    num_conflict_instances += add.total_memory_conflicts == 0 ? 0 : 1;
    total_iterations += add.loop_iterations;
    total_conflict_iterations += add.actual_conflict_iterations;
    total_mem_conflicts += add.total_memory_conflicts;
    instr_summary.set_summarize(add.instr_summary);
}
std::ostream &operator<<(std::ostream &os, const LoopTotalSummary &obj) {
    os << "{ \"LoopInstances\": " << obj.num_instances <<  ", " <<
          "\"ConflictingLoopInstances\": " << obj.num_conflict_instances <<  ", " <<
          "\"TotalLoopIterations\": " << obj.total_iterations <<  ", " <<
          "\"ConflictedIterations\": " << obj.total_conflict_iterations <<  ", " <<
          "\"TotalConflictingBytes\": " << obj.total_mem_conflicts <<  ", " <<
          "\"InstructionSummary\": " << obj.instr_summary <<
          " }\n";
    os << endl;
    return os;
}
