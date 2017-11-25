#include "test_object.h"
#include <stdexcept>
#include <iostream>

using namespace std;

TestObj all_tests;

bool testtestpass();
bool testtestfail();
bool testtesterror();

//bitset test
bool compressed_bits_test();


void TestObj::run_all() {
    bool has_failed = false;
    for (size_t i = 0; i < tests.size(); i++) {
        run_func(tests[i]);
        has_failed = has_failed || !test_valid;
    }
    if (has_failed) {
        cout << "\n\nSOME TESTS DID NOT PASS!!!\n" << endl;
    } else {
        cout << "\n\nALL TESTS PASSED.\n" << endl;
    }
}
void TestObj::run_func(test_ty test) {
    cur_test = test;
    cout.clear();
    try {
        cout << test.name << ": " << endl;

        test_valid = true;
        test.fn();

        if (test_valid) {
            cout << "PASSED" << endl;
        } else {
        }
    } catch (std::exception &except) {
        cout << "EXCEPTION RAISED\n" << except.what() << endl;
        test_valid = false;
    } catch (...) {
        cout << "threw some weird exception" << endl;
        test_valid = false;
    }
}

void TestObj::debug_print(std::string str) { cout << "Test print: " << str << endl; }

void TestObj::_assert(bool condition, string mystr) {
    if (!condition) {
        cout << "In function: " << cur_test.name << endl;
        cout << "FAILED " << mystr << endl;
        test_valid = false;
    }
}

bool testtestpass() { return true; }

bool testtestfail() { return false; }

bool testtesterror() { throw runtime_error("testtesterror raised error as expected"); }

inline void TestObj::collect_tests() {
#define _add_test(test) add_test(test, #test)
#ifdef TESTTEST
    _add_test(testtestpass);
    _add_test(testtestfail);
    _add_test(testtesterror);
#endif
    //compressed bits test
    _add_test(compressed_bits_test);
#undef _add_test
}

int main() {
    all_tests.collect_tests();
    all_tests.run_all();
}
