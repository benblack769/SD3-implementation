#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Types.h"
#include "PC_ID.h"
using namespace std;

typedef pair<int64_t,int64_t> PC_Pair;
class PointList{
protected:
    int64_t access_size;
    std::vector<int64_t> points;
public:
    PointList(){
        access_size = -1;
    }
    void add(int64_t cur_addr, int64_t cur_access_size){
        assert(access_size == -1 || access_size == cur_access_size);
        access_size = cur_access_size;
        points.push_back(cur_addr);
    }
    void merge_into(PointList & other){
        assert_equal_size(other);
        points.insert(points.end(),other.points.begin(),other.points.end());
    }
    vector<PC_Pair> enumerate_conflicts(PointList & other){
        assert_equal_size(other);
        vector<PC_Pair> res;
        res.reserve(points.size() * other.points.size());
        for(size_t i = 0; i < points.size(); i++){
            for(size_t j = 0; j < other.points.size(); j++){
                res.push_back(make_pair(points[i],other.points[j]));
            }
        }
        return res;
    }
    int16_t get_access_size(){
        return access_size;
    }
    void assert_equal_size(PointList & other){
        if(other.access_size != this->access_size){
            cerr << "differing access sizes" << this->access_size << "at address" << hex << this->points[0] << dec << "\t"
                                             << other.access_size << "at address" << hex << other.points[0] << dec << "\n";
        }
    }
};
