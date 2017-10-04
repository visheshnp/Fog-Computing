import matplotlib.pyplot as plt 
import numpy as np
import os
fig, ax = plt.subplots()

new_x = [0, 0, 0, 0, 0] #n_nodes
new_y = [0, 0, 0, 0, 0] #n_nodes

os.system("cp /home/vishesh/Desktop/Data/log_3set/0/coord_10.0.0.4:50051.txt /home/vishesh/cs8903/gmock_vivaldi/files/1.txt")
os.system("cp /home/vishesh/Desktop/Data/log_3set/1/coord_10.0.0.5:50052.txt /home/vishesh/cs8903/gmock_vivaldi/files/2.txt")
os.system("cp /home/vishesh/Desktop/Data/log_3set/2/coord_10.0.0.6:50053.txt /home/vishesh/cs8903/gmock_vivaldi/files/3.txt")
os.system("cp /home/vishesh/Desktop/Data/log_3set/3/coord_10.0.0.7:50054.txt /home/vishesh/cs8903/gmock_vivaldi/files/4.txt")
os.system("cp /home/vishesh/Desktop/Data/log_3set/4/coord_10.0.0.8:50055.txt /home/vishesh/cs8903/gmock_vivaldi/files/5.txt")

directory = os.listdir('/home/vishesh/cs8903/gmock_vivaldi/files')
os.chdir('/home/vishesh/cs8903/gmock_vivaldi/files')	
directory.sort()
#print directory

for t in range(500): #no of runs/lines
    i=0
    if t == 0:
        points, = ax.plot(new_x, new_y, marker='o', linestyle='None')
        ax.set_xlim(-100, 200) 
        ax.set_ylim(-100, 100) 
    else:
	    for f in directory:	#n_nodes/num of files
	        file = open(f,'r')
	        lines = file.readlines()
	        x_coord = (lines[t-1].split(','))[0].rstrip()
	        y_coord = (lines[t-1].split(','))[1].rstrip()
	       	new_x[i] = float(x_coord)
	       	new_y[i] = float(y_coord)
	        points.set_data(new_x, new_y)
	        i=i+1
    plt.pause(0.5)

print new_x
print new_y
