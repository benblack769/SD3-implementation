
'''
Need to test these cases:

* Parrellel independent of killed bits (read conflicts only)
* Parrellelism is dependent on killed bits
* Not parellel, with no killed bits
* Not parrelel, kill bits remove some of the conflicts

Also need some stress tests
'''
import os
from generator import Program


output_relative_path = "../test-data/conflict/"
os.makedirs(output_relative_path, exist_ok=True)


def sd3_example():
    prog  = Program()
    size = 4
    A = [prog.new_list(size)  for x in range(size)]
    B = [prog.new_list(size)  for x in range(size)]

    for i in range(2):
        prog.loop_start()
        for j in range(2):
            prog.loop_start()
            A[i][j].assign(2,prog.un_op(1,A[i][j-1]))
            B[i][j].assign(4,prog.un_op(3,A[i+1][j]))
            prog.loop_end()
        prog.loop_end()

    prog.generate_file(output_relative_path+"sd3_example.txt")


def mat_mul():
    prog  = Program()
    size = 50
    A = [prog.new_list(size)  for x in range(size)]
    B = [prog.new_list(size)  for x in range(size)]
    R = [prog.new_list(size)  for x in range(size)]

    for i in range(size):
        prog.loop_start()
        for j in range(size):
            prog.loop_start()
            R[i][j].assign(1,prog.constant())
            for k in range(size):
                prog.loop_start()
                R[i][j].assign(2,prog.bin_op(3,B[k][j],prog.bin_op(4,A[i][k],R[i][j])))
                prog.loop_end()
            prog.loop_end()
        prog.loop_end()

    prog.generate_file(output_relative_path+"mat_mul.txt")


sd3_example()
mat_mul()


#
