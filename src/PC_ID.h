#pragma once
#include <map>
#include "Types.h"
using namespace std;

enum MemAccessMode { READ = 0, WRITE = 1 };
class PC_ID{
public:
    int64_t pc;
    MemAccessMode accessmode;
    bool operator < (PC_ID other) const{
        return pc < other.pc || (pc == other.pc && accessmode < other.accessmode);
    }
};

template<class ItemType>
class access_mode_pair{
protected:
    ItemType read;
    ItemType write;
public:
    ItemType & get(AccessMode mode){
        if(mode == READ){
            return read;
        }
        else if(mode == WRITE){
            return write;
        }
        else{
            assert(false && "bad mode");
        }
    }
    ItemType & operator[](AccessMode mode){
        return get(mode);
    }
};
/*
template<class DataType>
class PC_Data{
public:
    typedef map<int64_t, DataType> container_type;
protected:
    access_mode_pair<container_type> data;
public:

    DataType & operator [](PC_ID identifier){
        return data[identifier.accessmode][identifier.pc];
    }
};
*/
