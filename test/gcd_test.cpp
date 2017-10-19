#include "dynamic_gcd.h"
#include "test_object.h"
#include <cassert>
#include <iostream>

void test_gcd() {
    t_assert(gcd(110, 5) == 5);
    t_assert(gcd(113, 5) == 1);
    t_assert(gcd(113, 2) == 1);
    t_assert(gcd(99, 121) == 11);
    t_assert(gcd(3, 1) == 1);
    t_assert(gcd(1, 1) == 1);
}

void print_stride(SparseStride stride) {
    cout << "stride: " << stride << endl;
    for (int i = 0; i < stride.end(); i++) {
        if (stride.is_in_stride(i)) {
            cout << "X";
        } else {
            cout << "O";
        }
    }
    cout << endl;
}
void debug_print_test_overlap(SparseStride stride1, SparseStride stride2) {
    print_stride(stride1);
    print_stride(stride2);

    cout << "num overlap calculated = " << num_overlap_locations(stride1, stride2) << endl;
}
void test_num_overlap_stride_stride() {
    SparseStride stride1(12, 4, 7, 4);
    SparseStride stride2(4, 4, 16, 4);
    debug_print_test_overlap(stride1, stride2);
    t_assert(num_overlap_locations(stride1, stride2) == 4);
}
void test_num_overlap_stride_block() {
    Block stride1(5, 15 + 1);
    SparseStride stride2(4, 4, 16, 4);
    t_assert(num_overlap_locations(stride2, stride1) == 4);
}
void test_num_overlap_block_block() {
    Block b1(10, 12 + 1);
    Block b2(11, 15 + 1);
    Block b3(12, 14 + 1);
    t_assert(num_overlap_locations(b1, b2) == 2);
    t_assert(num_overlap_locations(b2, b1) == 2);
    t_assert(num_overlap_locations(b3, b2) == 3);
    t_assert(num_overlap_locations(b2, b3) == 3);
}
