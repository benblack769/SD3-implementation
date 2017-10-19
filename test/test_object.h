#pragma once
#include <string>
#include <utility>
#include <vector>

typedef bool (*test_fn)();

struct test_ty {
    test_fn fn;
    std::string name;
};

class TestObj {
  protected:
    bool test_valid;
    test_ty cur_test;
    std::vector<test_ty> tests;

  public:
    void add_test(test_fn test, std::string func_name) {
        test_ty new_test = {test, func_name};
        tests.push_back(new_test);
    }
    void run_all();
    void debug_print(std::string str);
    void _assert(bool condition, std::string mystr);
    void collect_tests();

  protected:
    void run_func(test_ty fn);
    void fancy_print(std::string func_str);
};

extern TestObj all_tests;

#define _STR_HELPER(x) #x
#define TO_STR_CONST(x) _STR_HELPER(x)

#define t_assert(condition)                                                                                            \
    all_tests._assert(condition, "on line " TO_STR_CONST(__LINE__) ": the condition " #condition " returned false.")
#define t_debug(statement) all_tests.debug_print(#statement)
