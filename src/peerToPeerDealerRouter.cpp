#include <pthread.h>
#include <map>
#include <deque>
#include <queue>
#include <mutex>
#include <ctime>
#include <chrono>
#include "zhelpers.hpp"
#include "vivaldi.h"
#include "vivaldi.cpp"
#include "proto/vivaldi.pb.h"
#include "proto/vivaldi.pb.cc"
#include <iostream>
#include <fstream>

// Number of nodes to be contacted
// #define NUM_NODES 3

using namespace std;

Vivaldi* myVivaldiCoordinates;
mutex myVivaldiMutex;

string centralRepoIP;
vector<string> peerListIP; // ipPort
map<string, queue<double>> rttMap;
int maxRttStoredSize = 10;
string nodeId; // it is being init to ipport tuple

map<string, ofstream*> rtt_files;
map<string, ofstream*> coord_files;

void DeserializeNodeDetails(std::string nodeDetails) 
{
    CentralReply nodeDetailsReply;
    nodeDetailsReply.ParseFromString(nodeDetails);

    cout << "Recv: c_c:" << nodeDetailsReply.c_c() << endl;
    cout << "Recv: c_e:" << nodeDetailsReply.c_e() << endl;

    // Inits my nodes - random coords and received consts
    myVivaldiCoordinates = new Vivaldi(nodeDetailsReply.c_e(), nodeDetailsReply.c_c(),nodeId);

    cout << "INIT" << " ";
    myVivaldiCoordinates->print_my_location();

    deque<double> tempDeQueue(maxRttStoredSize, 0.1);
    queue<double> tempQueue(tempDeQueue);
    for(int i=0; i < nodeDetailsReply.list_nodes_size();i++)
    {
        peerListIP.push_back(nodeDetailsReply.list_nodes(i));
        // insert dummy rtt values
        // change this to new queue<>() if there are issues - same data
        rttMap.insert(std::pair<string,queue<double>>(nodeDetailsReply.list_nodes(i), tempQueue));
    }
}

void initDetailsFromCentralRepo(string nodeID){
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to central repo server…" << std::endl;
    socket.connect(centralRepoIP);

    zmq::message_t id (nodeID.size());

    char* node_id = (char*)nodeID.c_str();
    memcpy (id.data(), node_id, nodeID.size());
    std::cout << "Sending my nodeid " << nodeID << std::endl;
    socket.send (id);
    std::cout << "After Sending node id " << nodeID << std::endl;
    
    //  Get the reply.
    zmq::message_t reply;
    socket.recv (&reply);

    std::string node_details = std::string(static_cast<char*>(reply.data()),reply.size());
    DeserializeNodeDetails(node_details);
}

// string to vivaldi object
pair<Vivaldi::Coord,long long> myDeserializeUpdate(std::string workload) 
{

    RttMessage messageVivaldiRtt;
    pair<Vivaldi::Coord,long long> remoteCoordAndProcTime;
    // workload is of type message
    messageVivaldiRtt.ParseFromString(workload);

    string peerId = messageVivaldiRtt.sourceidentity();
    CoordLatencyError coordLatencyError = messageVivaldiRtt.coordlatencyerror();

    Vivaldi remote;
    remote._c._lat = coordLatencyError.lat();
    remote._c._lng = coordLatencyError.lng();
    remote._error = coordLatencyError.error();

    // update my coord
    std::lock_guard<std::mutex> lock(myVivaldiMutex);
    myVivaldiCoordinates->sample(remote._c, (rttMap.find(peerId)->second).back(), remote._error);

    remoteCoordAndProcTime.first = remote._c;
    remoteCoordAndProcTime.second = messageVivaldiRtt.processingtimeatremote();

    // return coord and proc time
    return remoteCoordAndProcTime;
}

// vivaldi object to string
std::string mySerializeVivaldi(long long processingTime) {

    std::string str_out;

    CoordLatencyError* coordLatencyError = new CoordLatencyError();

    {
        std::lock_guard<std::mutex> lock(myVivaldiMutex);

        coordLatencyError->set_lat(myVivaldiCoordinates->_c._lat);
        coordLatencyError->set_lng(myVivaldiCoordinates->_c._lng);
        coordLatencyError->set_error(myVivaldiCoordinates->_error);
    }

    // vivaldi message that incl rtt value
    RttMessage messageVivaldiRtt;
    messageVivaldiRtt.set_sourceidentity(nodeId);
    messageVivaldiRtt.set_processingtimeatremote(processingTime);
    messageVivaldiRtt.set_allocated_coordlatencyerror(coordLatencyError);
    messageVivaldiRtt.SerializeToString(&str_out);

    return str_out;
}

// init by thread
static void* peerTask(void *args)
{
    zmq::context_t context(1);
    zmq::socket_t peerSocket(context, ZMQ_DEALER);

    string peerId = *reinterpret_cast<std::string*>(args);
    peerSocket.connect("tcp://" + peerId);

    int countIterations = 0;
        
    while(1)
    {
        //  Tell the broker we're ready for work
        s_sendmore(peerSocket, ""); // delimiter

        // timestamp
        auto timeAtSend = std::chrono::high_resolution_clock::now();
        s_send(peerSocket, mySerializeVivaldi(0));

        //  Get workload from broker
        s_recv(peerSocket);     //  Envelope delimiter
        std::string workload = s_recv(peerSocket);
        auto elapsedTime = std::chrono::high_resolution_clock::now() - timeAtSend;

        long long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime/1000).count();
        pair<Vivaldi::Coord,long long> remoteCoordAndProcTime = myDeserializeUpdate(workload);
        long long rtt = elapsedTimeMicroseconds - remoteCoordAndProcTime.second;

        rttMap[peerId].pop();
        rttMap[peerId].push(rtt);
       
        countIterations++;
        
        if(countIterations % 100 == 0)
        {
            //cout << "Iteration count: " << countIterations << endl;
            *coord_files[nodeId] << myVivaldiCoordinates->print_my_lat() << "," << myVivaldiCoordinates->print_my_lng() << endl;
            *rtt_files[peerId] << dist(myVivaldiCoordinates->_c,remoteCoordAndProcTime.first) << "," << rtt << endl;
            myVivaldiCoordinates->print_my_location();
        }   
    }

    return NULL;
}

void init_file_vectors(string path)
{
    string filename = path + "/coord_" + nodeId + ".txt";
    coord_files[nodeId] = new ofstream(filename);

    for(int i=0; i<peerListIP.size(); i++)
    {
        string filename_rtt = path + "/rtt_" + nodeId + "-" + peerListIP[i] + ".txt";
        rtt_files[peerListIP[i]] = new ofstream(filename_rtt);
    }
}

int main(int argc, char** argv) {

    if( argc == 5 ) {
        printf("My ip is %s\n", argv[1]);
        printf("The port is %s\n", argv[2]);
        printf("The centralRepo-IpPort is %s\n", argv[3]);
        printf("The path to log files is %s\n", argv[4]);
    } else { cout<< "Port and centralRepo-IpPort and path to log files needs to be supplied.\n"; exit(0);}

    std::string ip = argv[1];
    std::string port = argv[2];
    centralRepoIP = string("tcp://") + string(argv[3]);

    nodeId = ip + ":" + port;
    initDetailsFromCentralRepo(nodeId);
    std::string pathToLogFile = argv[4];
    init_file_vectors(pathToLogFile);

    zmq::context_t context(1);
    zmq::socket_t broker(context, ZMQ_ROUTER);

    std::string bind_to = "tcp://*:" + port;
    cout << "Binding to: " << bind_to << endl;

    broker.bind(bind_to);

    pthread_t peerReqThreads[peerListIP.size()];    

    int i=0;
    for(auto& peerId : peerListIP) {
        pthread_create(peerReqThreads + i, NULL, peerTask, &peerId);
        i++;
    }

    while (1) {

        //  Next message gives us least recently used worker
        std::string identity = s_recv(broker);

        s_recv(broker);     //  Envelope delimiter
        std::string workload = s_recv(broker);     //  Response from worker
        auto timeAtRecv = std::chrono::high_resolution_clock::now();
   
        myDeserializeUpdate(workload);
    
        s_sendmore(broker, identity);
        s_sendmore(broker, "");

        auto elapsedTime = std::chrono::high_resolution_clock::now() - timeAtRecv;
        long long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
       
        s_send(broker, mySerializeVivaldi(elapsedTimeMicroseconds/1000));
    }

    for (int peerNum = 0; peerNum < peerListIP.size(); ++peerNum) {
        pthread_join(peerReqThreads[peerNum], NULL);
    }

    return 0;
}

