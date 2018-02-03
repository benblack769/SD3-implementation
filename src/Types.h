#pragma once
#include <ctime>
#ifdef _WIN32
typedef long long int64_t;
typedef unsigned long long uint64_t;
#else
//#ifdef __PIN__
//typedef unsigned long long uint64_t;
//#endif
#include <stdint.h>
#include <cstdlib>
#endif

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
