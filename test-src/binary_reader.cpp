#include "LoopStack.h"
#include <fstream>
#include <cassert>
#include <cstring>
using namespace std;

#define in_file cin

void single_loop_parser_run(){
    LoopStack loopstack;

    char line_type;
    do{
        in_file.read((char *)(&line_type),sizeof(line_type));
        if(line_type == 'M'){
            char read_write;
            int64_t ip;
            int64_t addr;
            in_file.read((char *)(&read_write),sizeof(read_write));
            in_file.read((char *)(&ip),sizeof(ip));
            in_file.read((char *)(&addr),sizeof(addr));
            MemAccessMode mode = read_write == 'R' ? READ : WRITE;
            //int64_t start = my_clock();
            loopstack.addMemAccess(addr,8,ip,mode);
            //tot_time +=
        }
        else if(line_type == 'L'){
            char change_type;
            int64_t loop_id;
            in_file.read((char *)(&change_type),sizeof(change_type));
            in_file.read((char *)(&loop_id),sizeof(loop_id));
            switch(change_type){
                case 'S': loopstack.loop_start(loop_id);break;
                case 'I': loopstack.iter_end(loop_id);break;
                case 'E': loopstack.loop_end(loop_id);break;
                default: assert(false);
            }
        }
    }while(line_type != 'E');
    loopstack.print_loop_dependencies();
}

int main(int argc,char ** argv){
    single_loop_parser_run();
}
