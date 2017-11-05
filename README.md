# Dynamic Data dependence analysis

This package attempts to efficiently find memory dependencies dynamically based
on the pairwise method and this paper: SD3: A Scalable Approach to Dynamic Data-Dependence Profiling

### Building the code

The code is built with CMake. Currently there is no real functionality, only tests.

### Running tests

A simple CMake build puts the binaries in the `bin` directory.

The `unit_test` executable is a simple custom unit test framework that
gives error messages about unit tests that do not pass.

Other tests, not really appropriate for unit testing are the `stride_detector_test`
and the `merge_test`.

These take input files that are a list of memory accesses, and spit out how they processed them.
