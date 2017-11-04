#include <iostream>
#include <fstream>
#include "StrideDetector.h"
#include "ConflictData.h"

typedef CompressedData<Stride> StrideTable;
typedef CompressedData<Point> PointTable;

const int test_pc = 123;
const MemAccessMode test_mem_acc_mode = READ;
const int access_size = 4;
const PC_ID test_pc_id(test_pc,test_mem_acc_mode);

void get_detector_strides(vector<int64_t> accesses, StrideTable & stride_table, PointTable & point_table){
    StrideDetector detector;
    int access_size = 4;
    for(size_t aidx = 0; aidx < accesses.size(); aidx++){
        int64_t mem_addr = accesses[aidx];

        cout << "access: " << mem_addr << ", StrideState: " << detector.getState() << endl;

        MemAccessType access_ty = detector.addAccess(mem_addr);
        if(access_ty == POINT){
            Block access_block(mem_addr,mem_addr+access_size);
            point_table.add(Point(access_block,test_pc_id));
        }
        else{
            int64_t stride = detector.getStride();
            SparseStride access_stride(mem_addr,1,stride,access_size);
            stride_table.add(Stride(access_stride,test_pc_id));
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
    if (argc != 3) {
        cout << "needs exactly 2 argument, 2 input filenames\n";
        exit(1);
    }
    char *filename1 = argv[1];
    char *filename2 = argv[2];
    vector<int64_t> accesses1 = file_lines(filename1);
    vector<int64_t> accesses2 = file_lines(filename2);
    StrideTable s_table1;
    StrideTable s_table2;
    PointTable p_table1;
    PointTable p_table2;
    get_detector_strides(accesses1,s_table1,p_table1);
    get_detector_strides(accesses2,s_table2,p_table2);
    p_table1.merge_into(p_table2);
    s_table1.merge_into(s_table2);
    p_table1.print();
    s_table1.print();
    
}
