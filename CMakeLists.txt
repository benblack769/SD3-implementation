cmake_minimum_required (VERSION 2.8)
project (StrideDetector C CXX)

set(CPP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(CPP_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


include_directories("${CPP_SOURCE_DIR}")

#add_executable(test ${CPP_SOURCE_DIR}/tester.cpp ${CPP_SOURCE_DIR}/StrideDetector.cpp  ${CPP_SOURCE_DIR}/PendingStrideTable.cpp ${CPP_SOURCE_DIR}/PendingPointTable.cpp ${CPP_SOURCE_DIR}/Stride.cpp ${CPP_SOURCE_DIR}/Point.cpp ${CPP_SOURCE_DIR}/Types.h)
#add_executable(test ${CPP_SOURCE_DIR}/test-new-in.cpp ${CPP_SOURCE_DIR}/StrideDetector.cpp  ${CPP_SOURCE_DIR}/PendingStrideTable.cpp ${CPP_SOURCE_DIR}/PendingPointTable.cpp ${CPP_SOURCE_DIR}/Stride.cpp ${CPP_SOURCE_DIR}/Point.cpp)
add_executable(test ${CPP_TEST_DIR}/gcd_test.cpp ${CPP_SOURCE_DIR}/dynamic_gcd.h ${CPP_SOURCE_DIR}/dynamic_gcd.cpp ${CPP_TEST_DIR}/test_object.h ${CPP_TEST_DIR}/test_object.cpp)

set_property(TARGET test PROPERTY CXX_STANDARD 98)
set_property(TARGET test PROPERTY CXX_STANDARD_REQUIRED ON)
