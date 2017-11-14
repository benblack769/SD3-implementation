# Dynamic Data dependence analysis

This package attempts to efficiently find memory dependencies dynamically based
on the pairwise method and this paper: SD3: A Scalable Approach to Dynamic Data-Dependence Profiling

### Building the code

The code is built with CMake.

A simple CMake build puts the binaries in the `bin` directory. For example, using make on linux:

    cmake . -DCMAKE_BUILD_TYPE=Release
    make

### Running tests


#### Main end-to-end test

In `test-scripts`, there are two important files.

* `conflicts.py`, which runs through loops and accesses data, and outputs that in a file in `test-data/conflict/`
* `test_runner.py`, which runs the files in `test-data/conflict/` throught the Data-Dependence algorithm, and puts the output in files in the `test-data/conflict-out/` folder

So to run this test completely, do

    cd test-scripts
    python3 conflicts.py
    python3 test_runner.py

#### Unit tests

The `unit_test` executable is a simple custom unit test framework that
gives error messages about unit tests that do not pass.

#### Other tests

Other tests, not really appropriate for unit testing are the `stride_detector_test`
and the `merge_test`.

These take input files that are a list of memory accesses, and spit out how they processed them. The files `test-data/OldTestInputs` are somewhat appropriate for input.
