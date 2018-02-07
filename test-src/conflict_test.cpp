#include "LoopStack.h"
#include <fstream>
#include <cassert>
using namespace std;

void single_loop_parser_run(char * filename){
    LoopStack loopstack;
    
    string first_item;
    ifstream file(filename);
    assert(file && "incorrect filename");
    while(file >> first_item){
        int64_t loop_id;
        if(first_item == "STA"){
            file >> hex >> loop_id;
            loopstack.loop_start(loop_id);
        }
        else if(first_item == "END"){
            file >> hex >> loop_id;
            loopstack.loop_end(loop_id);
        }
        else if(first_item == "ITR"){
            file >> hex >> loop_id;
            loopstack.iter_end(loop_id);
        }
        else{
            int64_t mem_access = atoi(first_item.c_str());
            int64_t instr;
            string read_string;
            file >> hex >> instr >> read_string;
            MemAccessMode mode = read_string == "READ" ? READ : WRITE;
            loopstack.addMemAccess(mem_access,4,instr,mode);
        }
    }
    loopstack.print_loop_dependencies(cout);
}

int main(int argc,char ** argv){
    if (argc != 2) {
        cout << "needs exactly 1 argument, input filename\n";
        exit(1);
    }
    single_loop_parser_run(argv[1]);
}
