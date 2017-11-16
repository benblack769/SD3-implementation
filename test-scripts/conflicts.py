'''
Need to test these cases:

* Parrellel independent of killed bits (read conflicts only)
* Parrellelism is dependent on killed bits
* Not parellel, with no killed bits
* Not parrelel, kill bits remove some of the conflicts

Also need some stress tests
'''
import os
import random
from generator import Program


output_relative_path = "../test-data/conflict/"
os.makedirs(output_relative_path, exist_ok=True)


def sd3_example():
    prog = Program()
    size = 4
    A = [prog.new_list(size)  for x in range(size)]
    B = [prog.new_list(size)  for x in range(size)]

    prog.loop_start(1)
    for i in range(2):
        prog.loop_start(2)
        for j in range(2):
            A[i][j].assign(2,prog.un_op(1,A[i][j-1]))
            B[i][j].assign(4,prog.un_op(3,A[i+1][j]))
            prog.iter_end(2)
        prog.loop_end(2)
        prog.iter_end(1)
    prog.loop_end(1)

    prog.generate_file(output_relative_path+"sd3_example.txt")


def mat_mul():
    '''
    Outer two loops should be parrelelizable.
    Also a good test that strides are being created and merging properly,
    or this will take a lot of memory and time to process
    '''
    prog  = Program()
    size = 50
    A = [prog.new_list(size)  for x in range(size)]
    B = [prog.new_list(size)  for x in range(size)]
    R = [prog.new_list(size)  for x in range(size)]

    prog.loop_start(1)
    for i in range(size):
        prog.loop_start(2)
        for j in range(size):
            prog.loop_start(3)
            R[i][j].assign(1,prog.constant())
            for k in range(size):
                R[i][j].assign(2,prog.bin_op(3,B[k][j],prog.bin_op(4,A[i][k],R[i][j])))
                prog.iter_end(3)
            prog.loop_end(3)
            prog.iter_end(2)
        prog.loop_end(2)
        prog.iter_end(1)
    prog.loop_end(1)


    prog.generate_file(output_relative_path+"mat_mul.txt")

def kill_bit_dep():

    prog  = Program()
    size = 50
    A = prog.new_list(size)
    b = prog.new()

    prog.loop_start(1)
    for i in range(size):
        b.assign(0,prog.constant())
        A[i].assign(1,prog.constant())
        A[i].assign(3,prog.bin_op(2,A[i],b))
        prog.iter_end(1)
    prog.loop_end(1)

    prog.generate_file(output_relative_path+"kill_bit_dep.txt")

def kill_bit_nested_loop():
    '''
    Kill bits must be translated through nested loops
    propery for the outer loop to be parrelelizable.
    '''
    prog  = Program()
    size = 10
    A = prog.new_list(size)
    b = prog.new()

    prog.loop_start(1)
    for i in range(size):
        b.assign(0,prog.constant())
        prog.loop_start(2)
        for j in range(size):
            A[j].assign(1,prog.constant())
            prog.iter_end(2)
        prog.loop_end(2)

        prog.loop_start(3)
        for j in range(size):
            b.assign(3,prog.bin_op(2,A[j],b))
            prog.iter_end(3)
        prog.loop_end(3)

        prog.iter_end(1)
    prog.loop_end(1)

    prog.generate_file(output_relative_path+"kill_bit_nested_loop.txt")


def conflicts_nested_loop():
    '''
    Reads and writes must be translated through nested loops
    propery for the outer loop to have conflicts.
    '''
    prog  = Program()
    size = 10
    A = prog.new_list(size)
    b = prog.new()

    prog.loop_start(1)
    for i in range(size):
        prog.loop_start(2)
        for j in range(size):
            b.assign(3,prog.bin_op(2,A[j],b))
            A[j].assign(8,prog.un_op(9,b))
            prog.iter_end(2)
        prog.loop_end(2)

        b.assign(0,prog.constant())
        prog.loop_start(3)
        for j in range(size):
            A[j].assign(1,prog.constant())
            prog.iter_end(3)
        prog.loop_end(3)

        prog.iter_end(1)
    prog.loop_end(1)

    prog.generate_file(output_relative_path+"conflicts_nested_loop.txt")


def false_read_write_dep():
    '''
    A fairly simple test that has a read-write conflict,
    but in theory is parrelellizable.
    '''
    prog  = Program()
    size = 10
    A = prog.new_list(size)
    B = prog.new_list(size)
    C = prog.new_list(size)

    prog.loop_start(1)
    for i in range(1,size):
        A[i].assign(3,prog.un_op(2,B[i-1]))
        B[i].assign(5,prog.un_op(4,C[i]))
        prog.iter_end(1)
    prog.loop_end(1)

    prog.generate_file(output_relative_path+"false_read_write_dep.txt")


def many_points():
    '''
    This is mainly a performance tests.  The idea
    is to hit the code with as many bad things as possible:
    1. Tons of points
    2. Lots of instructions
    3. Lots of loops
    4. A few nested loops

    Right now it has 1,2,3, but not 4.
    '''
    prog  = Program()
    size = 1000
    A = prog.new_list(size)

    prog.loop_start(1)
    for i in range(size):
        for l in range(3):
            prog.loop_start(l+101)
            for j in range(10):
                for k in range(10):
                    rand1 = random.randrange(i-5,size)
                    rand2 = random.randrange(i-5,size)
                    rand3 = random.randrange(0,i+1)
                    A[rand3].assign(2+k,prog.bin_op(1+k,A[rand1],A[rand2]))
                prog.iter_end(l+101)
            prog.loop_end(l+101)
        prog.iter_end(1)
    prog.loop_end(1)


    prog.generate_file(output_relative_path+"many_points.txt")

sd3_example()
mat_mul()
kill_bit_dep()
kill_bit_nested_loop()
conflicts_nested_loop()
false_read_write_dep()
many_points()
#
