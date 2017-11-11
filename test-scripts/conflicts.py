
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
    prog  = Program()
    size = 4
    A = [prog.new_list(size)  for x in range(size)]
    B = [prog.new_list(size)  for x in range(size)]

    prog.loop_start()
    for i in range(2):
        prog.loop_start()
        for j in range(2):
            A[i][j].assign(2,prog.un_op(1,A[i][j-1]))
            B[i][j].assign(4,prog.un_op(3,A[i+1][j]))
            prog.iter_end()
        prog.loop_end()
        prog.iter_end()
    prog.loop_end()

    prog.generate_file(output_relative_path+"sd3_example.txt")


def mat_mul():
    prog  = Program()
    size = 50
    A = [prog.new_list(size)  for x in range(size)]
    B = [prog.new_list(size)  for x in range(size)]
    R = [prog.new_list(size)  for x in range(size)]

    prog.loop_start()
    for i in range(size):
        prog.loop_start()
        for j in range(size):
            prog.loop_start()
            R[i][j].assign(1,prog.constant())
            for k in range(size):
                R[i][j].assign(2,prog.bin_op(3,B[k][j],prog.bin_op(4,A[i][k],R[i][j])))
                prog.iter_end()
            prog.loop_end()
            prog.iter_end()
        prog.loop_end()
        prog.iter_end()
    prog.loop_end()


    prog.generate_file(output_relative_path+"mat_mul.txt")

def kill_bit_dep():
    prog  = Program()
    size = 50
    A = prog.new_list(size)
    b = prog.new()

    prog.loop_start()
    for i in range(size):
        b.assign(0,prog.constant())
        A[i].assign(1,prog.constant())
        A[i].assign(3,prog.bin_op(2,A[i],b))
        prog.iter_end()
    prog.loop_end()

    prog.generate_file(output_relative_path+"kill_bit_dep.txt")

def false_read_write_dep():
    prog  = Program()
    size = 10
    A = prog.new_list(size)
    B = prog.new_list(size)
    C = prog.new_list(size)

    prog.loop_start()
    for i in range(1,size):
        A[i].assign(3,prog.un_op(2,B[i-1]))
        B[i].assign(5,prog.un_op(4,C[i]))
        prog.iter_end()
    prog.loop_end()

    prog.generate_file(output_relative_path+"false_read_write_dep.txt")


def many_points():
    prog  = Program()
    size = 100000
    A = prog.new_list(size)

    prog.loop_start()
    for i in range(size):
        prog.loop_start()
        for j in range(10):
            rand1 = random.randrange(i-5,size)
            rand2 = random.randrange(i-5,size)
            A[i].assign(2,prog.bin_op(1,A[rand1],A[rand2]))
            prog.iter_end()
        prog.loop_end()
        prog.iter_end()
    prog.loop_end()


    prog.generate_file(output_relative_path+"many_points.txt")

sd3_example()
mat_mul()
kill_bit_dep()
false_read_write_dep()
many_points()
#
