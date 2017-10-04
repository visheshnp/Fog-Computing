import os
import sys
from datetime import datetime
import subprocess
ip_port = str(sys.argv[1])
central_repo = str(sys.argv[2])
current_timestamp = datetime.now().strftime('%Y_%m_%d')
os.system("rm -r ~/build")
os.system("mkdir ~/build")
os.system("mkdir ~/log" + current_timestamp)
os.system("cd ~/build; cmake ~/src"+current_timestamp+";make")
os.system("cp ~/src" + current_timestamp + "/config.ini" + '\t'+ "~/build/config.ini")
if(ip_port.split(':')[0] == central_repo.split(':')[0]):
	#os.system("cd ~/build;./centralRepo" + '\t' + central_repo.split(':')[1])
	subprocess.Popen(["./build/centralRepo",central_repo.split(':')[1]])

os.system("cd ~/build;./peerTopeer" + '\t' + ip_port.split(':')[0] + '\t' + ip_port.split(':')[1] +'\t' + central_repo +'\t' + "~/log" + current_timestamp)