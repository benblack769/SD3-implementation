import subprocess
import os

input_data_folder = "../test-data/conflict/"
output_data_folder = "../test-data/conflict-out/"
for filename in os.listdir(input_data_folder):
    with open(output_data_folder+filename,'w') as outfile:
        subprocess.check_call(['../bin/conflict_test',input_data_folder+filename],stdout=outfile)
