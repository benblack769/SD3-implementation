#include "LoopStack.h"
#include "c_interface.h"

int64_t ptr_int(void * ptr){
    assert(sizeof(ptr) == sizeof(int64_t));
    return reinterpret_cast<int64_t>(ptr);
}
struct ParrDetector{
    LoopStack stack;
};
ParrDetector * create_detector(){
    return new ParrDetector();
}
void delete_detector(ParrDetector * detector){
    delete detector;
}
MemAccessMode mode_convert(MemoryAccessMode m){
    switch(m){
        case READ_MODE: return READ;
        case WRITE_MODE: return WRITE;
    }
    return READ;
}
void d_mem_access(ParrDetector * detector,void * mem_address,unsigned long long access_size,void * instruction, MemoryAccessMode mode){
    assert(instruction != NULL);
    assert(mem_address != NULL);
    assert(access_size  > 0 && access_size < 256);
    assert(mode == READ_MODE || mode == WRITE_MODE);
    detector->stack.addMemAccess(ptr_int(mem_address),access_size,ptr_int(instruction),mode_convert(mode));
}
void d_loop_end(ParrDetector * detector,void * loop_id){
    detector->stack.loop_end(ptr_int(loop_id));
}
void d_iteration_end(ParrDetector * detector,void * loop_id){
    detector->stack.iter_end(ptr_int(loop_id));
}
void d_loop_begin(ParrDetector * detector,void * loop_id){
    detector->stack.loop_start(ptr_int(loop_id));
}
void d_print_output(ParrDetector * detector){
    detector->stack.print_loop_dependencies();
}
