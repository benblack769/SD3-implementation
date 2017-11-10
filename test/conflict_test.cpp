#include "LoopStack.h"
#include <fstream>
#include <sstream>
using namespace std;

string slice(string orig,size_t start,size_t end){
    return string(orig.begin()+start,orig.begin()+end);
}
string slice_end(string orig,size_t start){
    return string(orig.begin()+start,orig.end());
}

void single_loop_parser_run(char * filename){
    LoopStack loopstack;
    
    string cur_line;
    ifstream file(filename);
    while(getline(file,cur_line)){
        if(slice(cur_line,0,5) == "START"){
            int loop_id = atoi(slice_end(cur_line,5+1).c_str());
            loopstack.iteration_start_signal(loop_id);
        }
        else if(slice(cur_line,0,3) == "END"){
            int loop_id = atoi(slice_end(cur_line,3+1).c_str());
            loopstack.iteration_end_signal(loop_id);
        }
        else{
            istringstream  line_data(cur_line);
            int64_t mem_access;
            int64_t instr;
            string read_string;
            line_data >> mem_access >> instr >> read_string;
            MemAccessMode mode = read_string == "READ" ? READ : WRITE;
            loopstack.addMemAccess(Point(mem_access,4,instr,mode));
        }
    }
}

int main(int argc,char ** argv){
    if (argc != 2) {
        cout << "needs exactly 1 argument, input filename\n";
        exit(1);
    }
    single_loop_parser_run(argv[1]);
}
