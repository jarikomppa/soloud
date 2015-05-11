import glob
import subprocess

for file in glob.glob("gen_*.py"):
    subprocess.call(["python",file])
    
