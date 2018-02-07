#pragma once
#ifndef __PIN__
#include <ctime>
#endif

#include <stdint.h>
inline int64_t iabs(int64_t num){
    return num >= 0 ? num : -num;
}
#ifndef __PIN__
inline int64_t my_clock(){
    timespec spc;
    clock_gettime(CLOCK_MONOTONIC,&spc);
    return spc.tv_nsec + 1000000000LL*spc.tv_sec;
}
#else
inline int64_t my_clock(){
    return 0;
}
#endif

//#include <unordered_map>
#define unordered_map map
