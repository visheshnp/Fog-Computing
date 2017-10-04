import subprocess
import data
for i in range(0,int(data.n_nodes)):
	subprocess.Popen(["python","local_script.py", str(i)])

