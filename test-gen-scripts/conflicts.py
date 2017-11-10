
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

'''
def read_conflicts_only():
    program  = Program()
    size = 4
    A = program.new_list(size)
    sum = program.new()

    for i in range(size-1):
        program.loop_start()
        A[i].assign(A[i].bin_op(A[j],))
        program.loop_end()

    program.generate_file("sd3_example.txt")
'''

sd3_example()





#
