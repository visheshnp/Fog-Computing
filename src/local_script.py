from __main__ import * 
import os
import data
import sys
from datetime import datetime
import subprocess
i = int(sys.argv[1])
#print "Value of i is" + '\t' + i
ssh_key = str(data.final_ssh_keys[i])
node_user = str(data.final_node_users[i])
ip_addr = str(data.final_ip_addrs[i])
current_timestamp = datetime.now().strftime('%Y_%m_%d')

clean_script = "\"rm -r ~/src"+current_timestamp +"\""  #"; p=$(pgrep peerTopeer); kill $p; p=$(pgrep centralRepo); kill $p;\"";
ssh_clean="ssh -q -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -i" + '\t' + ssh_key + '\t' + node_user + "@" + ip_addr + '\t' + clean_script
os.system(ssh_clean)

scp="scp -q -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -i" + '\t' + ssh_key + '\t' + "-r ~/cs8903/src/" + '\t' + node_user + "@" + ip_addr + ":src" + current_timestamp
os.system(scp)
ssh="ssh -q -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -i" + '\t' + ssh_key + '\t' + node_user + "@" + ip_addr + '\t' + "\"python ~/src"+current_timestamp+"/actual_local.py" + '\t'+ str(data.final_dict_ipports[i+1]) + '\t' + str(data.final_central_ipport) +"\""
os.system(ssh)
