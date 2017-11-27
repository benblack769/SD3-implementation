cmake_minimum_required (VERSION 2.8)
project (StrideDetector C CXX)

set(CPP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(CPP_TEST_DIR ${PROJECT_SOURCE_DIR}/test-src)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


include_directories("${CPP_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

#add_executable(unit_test ${CPP_TEST_DIR}/test_object.h ${CPP_TEST_DIR}/test_object.cpp  ${CPP_SOURCE_DIR}/FindIntersect.h ${CPP_TEST_DIR}/FindIntersectTest.cpp ${CPP_SOURCE_DIR}/CompressedBits.cpp  ${CPP_SOURCE_DIR}/CompressedBits.h ${CPP_TEST_DIR}/CompressedBitsTest.cpp)
add_executable(conflict_test ${CPP_TEST_DIR}/conflict_test.cpp ${CPP_SOURCE_DIR}/CompressedBits.cpp  ${CPP_SOURCE_DIR}/CompressedBits.h ${CPP_SOURCE_DIR}/LoopInstance.h ${CPP_SOURCE_DIR}/LoopInstance.cpp ${CPP_SOURCE_DIR}/LoopStack.h ${CPP_SOURCE_DIR}/LoopStack.cpp ${CPP_SOURCE_DIR}/PC_ID.h ${CPP_SOURCE_DIR}/Dependence.cpp)

#set_property(TARGET unit_test PROPERTY CXX_STANDARD 98)
#set_property(TARGET unit_test PROPERTY CXX_STANDARD_REQUIRED ON)

set_property(TARGET conflict_test PROPERTY CXX_STANDARD 98)
set_property(TARGET conflict_test PROPERTY CXX_STANDARD_REQUIRED ON)
