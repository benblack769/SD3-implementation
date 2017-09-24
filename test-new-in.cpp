#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


struct MemAccess{
    size_t instruction;
    size_t memory;
};

vector<MemAccess> all_accesses;

vector<MemAccess> load_accesses(char * filename){
    vector<MemAccess> res;
    
    ifstream myfile(filename);
    
    if (myfile.is_open()) {
        size_t ins,mem;
        while ((myfile >> ins) && (myfile >> mem)) {
            MemAccess acc = {ins,mem};
            res.push_back(acc);
        }
    }
    return res;
}

int main(int argc, char ** argv){
    if(argc != 2){
        cout << "needs exactly 1 argument, a input filename\n";
        exit(1);
    }
    char * filename = argv[1];
    vector<MemAccess> accesses = load_accesses(filename);
    
}
