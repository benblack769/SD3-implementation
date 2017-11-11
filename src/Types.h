#pragma once

#ifdef _WIN32
typedef long long int64_t;
typedef unsigned long long uint64_t;
#else
#include <cstdlib>
#endif

inline int64_t iabs(int64_t num){
    return num >= 0 ? num : -num;
}
