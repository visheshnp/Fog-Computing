import os
from collections import OrderedDict
file = open("config.ini",'r')
lines = file.readlines()

c_e= (lines[0].split('='))[1].rstrip()
c_c= (lines[1].split('='))[1].rstrip()
n_nodes = (lines[2].split('='))[1].rstrip()

final_dict_ipports = OrderedDict()
list_ip_port = (lines[3].split('='))
del list_ip_port[0]
list_ip_ports = list_ip_port[0].split(',')
list_ip_ports = [s.rstrip() for s in list_ip_ports]
for i in range(0,len(list_ip_ports)):
	key	=int((list_ip_ports[i].split('-'))[0])
	value = (list_ip_ports[i].split('-'))[1]
	final_dict_ipports[key]=value

final_node_mapping = OrderedDict()
node_map = (lines[4].split('='))
del node_map[0]
node_maps = node_map[0].split(',')
node_maps = [s.rstrip() for s in node_maps]

for i in range(0,len(node_maps)):
	key = int((node_maps[i].split('-'))[0])
	#for j in ((node_maps[i].split('-'))[1]):
	node_list = ((node_maps[i].split('-'))[1]).split(';')
	final_node_mapping[key] = node_list

central_ipport = (lines[5].split('='))[1]
final_central_ipport = central_ipport.rstrip()
 
ssh_key = (lines[6].split('='))
del ssh_key[0]
ssh_keys = ssh_key[0].split(',')
final_ssh_keys= [s.rstrip() for s in ssh_keys]
#print final_ssh_keys

ip_addr = (lines[7].split('='))
del ip_addr[0]
ip_addrs = ip_addr[0].split(',')
final_ip_addrs = [s.rstrip() for s in ip_addrs]

node_user = (lines[8].split('='))
del node_user[0]
node_users = node_user[0].split(',')
final_node_users = [s.rstrip() for s in node_users]
