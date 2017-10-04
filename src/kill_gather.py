from __main__ import * 
import os
import data
import sys
from datetime import datetime
import subprocess
i = int(sys.argv[1])
ssh_key = str(data.final_ssh_keys[i])
node_user = str(data.final_node_users[i])
ip_addr = str(data.final_ip_addrs[i])
current_timestamp = datetime.now().strftime('%Y')

clean_script = "\"killall -9 centralRepo; killall -9 peerTopeer\"";
ssh_clean="ssh -q -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -i" + '\t' + ssh_key + '\t' + node_user + "@" + ip_addr + '\t' + clean_script
os.system(ssh_clean)

scp="scp -q -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -i" + '\t' + ssh_key + '\t' + " -r " + node_user + "@" + ip_addr + ":log" + current_timestamp + " ~/cs8903/build/log/" + str(i)
os.system(scp)