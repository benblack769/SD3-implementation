#include "dynamic_gcd.h"
#include "test_object.h"
#include <cassert>
#include <iostream>

int64_t gcd(int64_t a, int64_t b);

int64_t num_overlap(SparseStride one, SparseStride other);

void test_gcd() {
    t_assert(gcd(110, 5) == 5);
    t_assert(gcd(113, 5) == 1);
    t_assert(gcd(113, 2) == 1);
    t_assert(gcd(99, 121) == 11);
    t_assert(gcd(3, 1) == 1);
    t_assert(gcd(1, 1) == 1);
}
void test_block_intersect() {
    Block b1 = {10, 12 + 1, 5};
    Block b2 = {11, 15 + 1, 8};
    num_conflict_b(b1, b2);
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

    int64_t num = num_overlap(stride1, stride2);

    t_assert(true);
}
void test_num_overlap() {
    SparseStride stride1(12, 4, 7, 4);
    SparseStride stride2(4, 4, 16, 4);
    debug_print_test_overlap(stride1, stride2);
    SparseStride stride3();
    SparseStride stride4();
    t_assert(num_overlap(stride1, stride2));
}
