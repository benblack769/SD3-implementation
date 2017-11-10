#pragma once
#include <map>
#include <cassert>
#include "Types.h"
using namespace std;

enum MemAccessMode { INVALID=-1, READ = 0, WRITE = 1 };
class PC_ID{
public:
    PC_ID():
        pc(-1),
        accessmode(INVALID){}
    PC_ID(int64_t in_pc, MemAccessMode in_acc_mode):
        pc(in_pc),
        accessmode(in_acc_mode){}
    bool operator < (PC_ID other) const {
        return pc < other.pc || (pc == other.pc && accessmode < other.accessmode);
    }
    bool operator == (PC_ID other) const {
        return pc == other.pc && accessmode == other.accessmode;
    }
    bool is_invalid(){
        return this->pc == INVALID;
    }
    MemAccessMode get_acc_mode()const{return accessmode;}
protected:
    int64_t pc;
    MemAccessMode accessmode;
};
const PC_ID null_id(-1,INVALID);

/*
template<class ItemType>
class access_mode_pair{
protected:
    ItemType read;
    ItemType write;
public:
    ItemType & get(MemAccessMode mode){
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
    ItemType & operator[](MemAccessMode mode){
        return get(mode);
    }
};*/
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
