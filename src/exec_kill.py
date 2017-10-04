import subprocess
import data
for i in range(0,int(data.n_nodes)):
	subprocess.Popen(["python","kill_gather.py", str(i)])