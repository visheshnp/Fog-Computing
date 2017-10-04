from numpy import *
import math
import matplotlib.pyplot as plt
import os

file = open('/home/vishesh/Desktop/Data/log_2set/1/rtt_10.0.0.5:50052-10.0.0.7:50054.txt','r')
lines = file.readlines()

t = linspace(0,len(lines),num=len(lines))
virtual = []
actual = []

for i in range(0,len(lines)):
	virtual.append((lines[i].split(','))[0].rstrip())
	actual.append((lines[i].split(','))[1].rstrip())

print virtual
print t
plt.axis([0,180,0,100])
plt.plot(t,virtual,'r') # plotting t,a separately 
plt.plot(t,actual,'b') # plotting t,b separately 
plt.show()