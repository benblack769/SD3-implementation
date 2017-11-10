#pragma once
#include "LoopInstance.h"
#include <cassert>
#include <list>
#include "Dependence.h"

using namespace std;

class LoopStack{
protected:
    bool top_loop_ended;
    list<LoopInstance> stack;
    map<int64_t, vector<Dependence> > loop_dependencies;
public:
    LoopStack(){
        top_loop_ended = false;
    }
    void addMemAccess(Point p){
        stack.front().addMemAccess(p);
    }
    void iteration_end_signal(int64_t loop_id){
        assert(stack.size() != 0);
        assert(stack.back().get_loop_id() == loop_id);
        if(top_loop_ended){
            stack.back().loop_end(loop_dependencies[stack.back().get_loop_id()]);
            stack.pop_back();
        }
        else{
            stack.back().iteration_end();
        }
        top_loop_ended = true;
    }
    void iteration_start_signal(int64_t loop_id){
        if(top_loop_ended){
            assert(stack.back().get_loop_id() == loop_id);
        }
        else{
            stack.push_back(LoopInstance(loop_id));
        }
        
        top_loop_ended = false;
    }
};
