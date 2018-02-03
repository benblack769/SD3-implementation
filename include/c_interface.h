#pragma once
extern "C" {

struct ParrDetector;
enum MemoryAccessMode {READ_MODE,WRITE_MODE};

ParrDetector * create_detector();
void delete_detector(ParrDetector * detector);
void d_mem_access(ParrDetector * detector,void * mem_address,unsigned long long access_size,void * instruction, MemoryAccessMode mode);
void d_loop_end(ParrDetector * detector,void * loop_id);
void d_iteration_end(ParrDetector * detector,void * loop_id);
void d_loop_begin(ParrDetector * detector,void * loop_id);
void d_print_output(ParrDetector * detector,char * filename);
}
