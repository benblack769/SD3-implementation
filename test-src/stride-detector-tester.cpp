#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "Point.h"
#include "StrideDetector.h"

using namespace std;

void print_detector_results(vector<int64_t> accesses){
    StrideDetector detector;
    int access_size = 4;
    for(size_t aidx = 0; aidx < accesses.size(); aidx++){
        int64_t mem_addr = accesses[aidx];

        cout << "access: " << mem_addr << ", StrideState: " << detector.getState() << endl;

        MemAccessType access_ty = detector.addAccess(mem_addr);
        if(access_ty == POINT){
            cout << "POINT: " << Block(mem_addr,mem_addr+access_size) << endl;
        }
        else{
            int64_t stride = detector.getStride();
            cout << "STRIDE: " << SparseStride(mem_addr,1,stride,access_size)  << endl;
        }
    }
}

vector<int64_t> file_lines(char *filename){
    vector<int64_t> lines;

    ifstream myfile(filename);

    int64_t fileLine = 0;

    if (myfile.is_open()) {
        while (myfile >> fileLine) {
            lines.push_back(fileLine);
        }
    }
    return lines;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "needs exactly 1 argument, a input filename\n";
        exit(1);
    }
    char *filename = argv[1];
    vector<int64_t> accesses = file_lines(filename);
    print_detector_results(accesses);
}
