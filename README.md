-----------------
README
-----------------

To login into the machines - subscription: Network Coordinates (65220f37-3215-4898-b18d-363c624d6cbe)
Machines named:
i:1 to 7
vivaldi-test-i
username: vivaldi-test-i
ssh key name: vivaldi-test-i_azure


Installing libraries:
---------------------

This will install the software packages required along with protobuf and libzmq

To install dependencies on the machine, login into the machine and run script:
>> sudo sh install_dependencies.sh

or use this:
>> ssh -q -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -i vivaldi-test-i vivaldi-test-i@ip_addr "sudo sh install_dependencies.sh"


Configuration:
----------------

config.ini defines the node mappings and the ip addresses.
ip addresses have to be public so that we can ssh into the machines.
ip addresses have to be private ones so that they can contact each other.
The number of nodes have to match the number of mappings given, ssh keys given and the ip addresses.
The mappings determine which nodes interact with each other.
The ip:port tuple given as the central repo is used for running the central repo on only one.



Running scripts:
----------------
>> python exec_nodes.py

Running this:
Copies the code from the current src folder to the remote machines.
Compiles the code on the remote machine.
Runs the central repository server on one of the nodes.



Collecting log data from each node:
-----------------------------------

>> python exec_kill.py

Running this kills the central repo and the peer to peer tasks running on the remote machines.
Post which, it gathers the logged data.
Ensure that the name given here for the log files matches the one given while running the scripts.


Plotting graphs:
----------------
The files would be present in a folder
The folder would have n folders for n nodes.
Each folder would contain one coordinates file and x number of rtt files. Where x is the number of nodes the current node talks to.

>> python plot_final.py 
>> python rtt_plot.py 


Graphs:
-----------
Around 6 tests with 5 different VM's were conducted to test the performance in various scenarios. The delay for every link beween every node is set to 60 ms. The description of the scenario along with graphical results and explanation is as follows :-

1) All 5 VM's start with random coordinates generated as a hash of its nodeId. After 50000 iterations, the nodes converge to a pentagon shaped figure as shown below. The RTT graphs depict the variation between the actual RTT(blue) calculated using timestamps and measured RTT(red) using the Vivaldi algorithm (time(ms) vs iterations/100). 

   <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/coordinates_all_random.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/coordinates_all_random.png" width="400" height="400">

   Measurements between nodes (2,3) and (4,5) are depicted below.

   <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/2_3_all_random.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/2_3_all_random.png" width="400" height="400">  <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/4_5_all_random.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/4_5_all_random.png" width="400" height="400"> 
  
2)  All 5 VM's start with same coordinates. After 70000 iterations, the nodes converge to a pentagon shaped figure as shown earlier. 

   Measurements between nodes (2,3) and (4,5) are depicted below.

   <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/2_3_coord_same.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/2_3_coord_same.png" width="400" height="400">  <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/4_5_coord_same.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/4_5_coord_same.png" width="400" height="400">  
  
3) The mapping of the 5 nodes is changed, and each node talks to a different set of nodes. For this test, the mapping is as follows :-

	1 -> 2,3,4

	2 -> 1,4,5

	3 -> 1,4,5 

	4 -> 1,2,3 

	5 -> 2,3

  The nodes converge in a different manner as shown below. It can be observed that since nodes 2,3 have the same mapping, they appear to be overlapping in position on the coordinates map (near 20,20).
  
  <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/node_3_mappings.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/node_3_mappings.png" width="400" height="400">
  
  The RTT measurements between nodes (1,4) , (3,2) and (5,3) are as follows :-
  
  <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/1_4_3_mappings.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/1_4_3_mappings.png" width="400" height="400"> <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/3_2_3_mappings.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/3_3_3_mappings.png" width="400" height="400">
  
  <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/5_3_3_mappings.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/5_3_3_mappings.png" width="400" height="400">  
  
 4) Out of the 5 nodes, 2 nodes were allowed to update while the other 3 remained static.
 
  In this experiment,the 5 nodes took about 80000 iterations to converge to the pentagon shape. The RTT measurements between  nodes (1,4) and (2,5) are as shown.
 
  <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/1_4_2_set.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/1_4_2_set.png" width="400" height="400"> <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/2_5_2_set.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/2_5_2_set.png" width="400" height="400">
  
 5) Only 3 nodes were considered, and they all started with initial random coordinates. The nodes converge to something close to an equilateral triangle as shown below. 
  
  <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/3_random.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/3_random.png" width="400" height="400"> 
  
  The RTT measurements between nodes  (1,2) and (2,3) are as follows :-
    
  <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/1_2_3random.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/1_2_3random.png" width="400" height="400"> <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/2_3_3random.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/2_3_3random.png" width="400" height="400">
   
 6) Only 2 nodes were considered, and they all started with initial random coordinates. The 2 nodes position themselves as shown below and the RTT graph is as follows :-
   
  <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/2_nodes.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/2_nodes.png" width="400" height="400"> <img src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/1_2_2_random.png" data-canonical-src="https://github.com/visheshnp/Fog-Computing/blob/master/Graphs/1_2_2_random.png" width="400" height="400"> 
