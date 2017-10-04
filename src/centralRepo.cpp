#include <string>
#include <map>
#include <zmq.hpp>
#include <iostream>
#include <unistd.h>
#include <bits/stdc++.h>
#include "proto/vivaldi.pb.h"
#include "proto/vivaldi.pb.cc"
#include "vivaldi.h"
#include "vivaldi.cpp"

//#define NUM_NODES 3

using namespace std;

map<string, CentralReply> node_values;

vector<vector<string>> iport_v;  
int n_nodes;
double c_c;
double c_e;
map<int,string> ip_port;

void read_from_config()
{
    ifstream config_file;
    config_file.open("build/config.ini");
    if(!config_file.is_open())
    {
        cout << "File not present!" << endl;
        exit(-1);
    }    
    map<int,vector<string>> node_map;
    
    vector<string> friends;
    string str;
   
    while(getline(config_file,str))
    {
        auto start = 0U;
        auto end = str.find('=');
        //cout << str.substr(start,end-start) << endl;
        if(str.substr(start,end-start).compare("n_nodes")==0)
        {   
            string node_info = str.substr(end+1);
            auto start_id = 0U;
            auto end_id = str.find('=');
            n_nodes = stod(node_info);
            //cout<< n_nodes << endl;
        }

          if(str.substr(start,end-start).compare("c_e")==0)
        {   
            string node_info = str.substr(end+1);
            auto start_id = 0U;
            auto end_id = str.find('=');
            c_e = stod(node_info);
            //cout<< n_nodes << endl;
        }

          if(str.substr(start,end-start).compare("c_c")==0)
        {   
            string node_info = str.substr(end+1);
            auto start_id = 0U;
            auto end_id = str.find('=');
            c_c = stod(node_info);
            //cout<< n_nodes << endl;
        }

      if(str.substr(start,end-start).compare("nodes_ipaddr_ports")==0)
            {
                string info = str.substr(end+1);
                int count = 0;
                auto start1 = 0U;
                auto end1 = info.find(','); 
                
                while(count < n_nodes)
                {
                string node = info.substr(start1,end1 - start1);
                int node_id = stoi(node.substr(0U, node.find("-")));
                ip_port[node_id] = node.substr(node.find("-")+1); 
            
                //cout << node.substr(node.find("-")+1)  << endl;
                //start1 = end1 +1;
                info = info.substr(end1+1);
                //cout << info << endl;
                end1 = info.find(',');

                count++;

                }

                //cout << n_nodes << endl;

                //cout << ip_port[2] << endl;
            }
        

        if(str.substr(start,end-start).compare("node_mapping")==0)
            {

                string info_map = str.substr(end+1);
                //cout << info_map << endl;
                auto start_map = 0U;
                int count_nodes =0;
                auto end_map = info_map.find(',');
                int count1 = 0;

            while(count_nodes < n_nodes)
            {
                //cout << end_map << endl;
                string node_mapdata = info_map.substr(start_map,end_map-start_map);
               // cout << node_mapdata << endl;
                int nodemap_id = stoi(node_mapdata.substr(0U, node_mapdata.find("-")));

                string node_friends = node_mapdata.substr(node_mapdata.find("-")+1);
                //cout << "Node friends" << endl;
                // << node_friends << " " ;
                //cout << endl;

                auto start_friend = 0U;
                auto end_friend = node_friends.find(';');
                
                int semicolon_count = count(node_friends.begin(),node_friends.end(),';');
                
                    while(count1 <= semicolon_count)
                    {
                            string friend_detail = node_friends.substr(start_friend,end_friend-start_friend);
                            friends.push_back(friend_detail);

                            node_friends = node_friends.substr(end_friend+1);
                            end_friend = node_friends.find(';');

                            //cout << "friend_detail" << endl;
                            //cout << friend_detail << " " ;
                            //cout << endl;
                            //cout << node_friends.substr(start_friend,end_friend-start_friend) << endl;
                            count1++;

                    }

                count1=0;   
                (iport_v).push_back(friends);
                friends.clear();

                info_map = info_map.substr(end_map+1);
                end_map = info_map.find(',');

                    //cout << nodemap_id << endl;
                count_nodes++;
            }   
            
                for(int i=0; i < iport_v.size();i++)
                {
                    for(int j=0; j < iport_v[i].size();j++)
                    {
                        iport_v[i][j] = ip_port[stoi(iport_v[i][j])];
                    }
                }

               /// for(int i=0; i < iport_v.size();i++)
                //    for(int j=0; j < iport_v[i].size();j++)
                 //       cout << iport_v[i][j] << endl;
                //  cout << nodemap_id << endl;
                        
            }
    }

}

CentralReply createCentralReply(double c_e, double c_c, int index){

    CentralReply reply;
    reply.set_c_e(c_e);
    reply.set_c_c(c_c);
    string* nodes_list;
   // cout << c_c << endl;
   // cout << c_e << endl;
    for(int i=0; i< iport_v[index].size(); i++)
    {
        //cout << iport_v[index].size() << endl;
        nodes_list = reply.add_list_nodes();
        *nodes_list = iport_v[index][i];
    }

    return reply;
}

void init_node_value_map() 
{
    for(int i=0; i < n_nodes;i++)
    {
         string addr = ip_port[i+1];   
         //cout << addr << endl;
         node_values.insert(std::pair<string,CentralReply>(addr, createCentralReply(c_e, c_c, i)));
    }

}

std::string findandSerialize(string id)
{
    map<string, CentralReply>::iterator it = node_values.find(id);
    cout << id << endl;
    if(it != node_values.end())
    {
    std::string str_out;
    (it->second).SerializeToString(&str_out);
    return str_out;   
    }
    
    else
    {
    cout << "Node not found in map" << id << endl;
    return "ERROR";
    }
}

int main (int argc, char** argv) 
{
    if( argc == 2 ) {
        printf("The port is %s\n", argv[1]);
    } else { cout<< "Enter Port for Central Repo 5555\n"; exit(0);}

    std::string port = argv[1];
    std::string bind_to = "tcp://*:" + port;
    cout << "Binding to: " << bind_to << endl;

    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind (bind_to);

    read_from_config();
    init_node_value_map();

    while (true) 
    {
        zmq::message_t id;
        socket.recv (&id);

        std::string node_id = std::string(static_cast<char*>(id.data()),id.size());
        std::cout << "Received Id " << node_id << std::endl;
 
        std::string str_out = findandSerialize(node_id);
        if (str_out.compare("ERROR") == 0)
        {
           // str_out = "Incorrect ID!";
            cout << "Details not present!" << endl;
        }
        
        else
        {
            zmq::message_t reply (str_out.size());
            memcpy (reply.data(), str_out.c_str(), str_out.size());
            socket.send (reply);
            // cout << "Sent node details" << str_out << endl;

        }    
    }

    return 0;
}
