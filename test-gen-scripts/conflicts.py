
'''
Need to test these cases:

* Parrellel independent of killed bits (read conflicts only)
* Parrellelism is dependent on killed bits
* Not parellel, with no killed bits
* Not parrelel, kill bits remove some of the conflicts

Also need some stress tests
'''
import os


output_relative_path = "../test-data/conflict/"
os.makedirs(output_relative_path, exist_ok=True)

NULL_LOC_ID = -1
class Program:
    def __init__(self):
        self.locations = []
        self.new_mem_loc = 0
        self.instruction_ids = dict()
        self.cur_instr_alloc = 100000
    def mem_access_occured(self,loc_id,instruction,was_read):
        if loc_id != NULL_LOC_ID:
            read_str = "READ" if was_read else "WRITE"
            self.locations.append("{}\t{}\t{}".format(loc_id,instruction,read_str))
    def read_occured(self,mem_loc,instr):
        return self.mem_access_occured(mem_loc.loc_id, instr, True)
    def write_occured(self,mem_loc,instr):
        return self.mem_access_occured(mem_loc.loc_id, instr, False)
    def generate_file(self,filename):
        with open(output_relative_path+filename,'w') as out_file:
            out_file.write("\n".join(self.locations))
    def new(self):
        self.new_mem_loc += 1
        return MemLocation(self,self.new_mem_loc)
    def new_list(self,size):
        return [self.new() for i in range(size)]
    def loop_start(self):
        self.locations.append("START")
    def loop_end(self):
        self.locations.append("END")
    def bin_op(instruction_id,mem_loc1,mem_loc2):
        self.read_occured(mem_loc1,self.instruction(instruction_id))
        self.read_occured(mem_loc2,self.instruction(instruction_id+0xcccccccc))
        return MemLocation(self,NULL_LOC_ID)
    def un_op(self,instruction_id,mem_loc1):
        self.read_occured(mem_loc1,self.instruction(instruction_id))
        return MemLocation(self,NULL_LOC_ID)
    def instruction(self,instr_id):
        if instr_id in self.instruction_ids:
            return self.instruction_ids[instr_id]
        else:
            self.cur_instr_alloc += 1
            self.instruction_ids[instr_id] = self.cur_instr_alloc
            return self.instruction_ids[instr_id]

class MemLocation:
    def __init__(self,program,loc_id):
        self.program = program
        self.loc_id = loc_id
    def assign(self,instruction,value_assign):
        self.program.write_occured(self,instruction)


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

    prog.generate_file("sd3_example.txt")

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
