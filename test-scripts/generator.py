
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
        with open(filename,'w') as out_file:
            out_file.write("\n".join(self.locations))
    def new(self):
        self.new_mem_loc += 4
        return MemLocation(self,self.new_mem_loc)
    def new_list(self,size):
        return [self.new() for i in range(size)]
    def loop_start(self,loop_id):
        self.locations.append("STA {}".format(loop_id))
    def iter_end(self,loop_id):
        self.locations.append("ITR {}".format(loop_id))
    def loop_end(self,loop_id):
        self.locations.append("END {}".format(loop_id))
    def bin_op(self,instruction_id,mem_loc1,mem_loc2):
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
    def constant(self):
        return MemLocation(self,NULL_LOC_ID)

class MemLocation:
    def __init__(self,program,loc_id):
        self.program = program
        self.loc_id = loc_id
    def assign(self,instruction,value_assign):
        self.program.write_occured(self,instruction)
