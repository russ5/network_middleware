#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "mw_internal.h"
#include "mw_public.h"

// User Function
int ringSetup(int nodeId, char * configPath) {  // Include config path here?
    struct Config * machines;                   // Struct for storing the config file
    int next_node, prev_node;                   // Track the node-1 and the node+1 from this node
    int comIds[64];                             // Store the socket IDs for other nodes

    if(configPath != "") {
       machines = readConfig(configPath);
       // Determine nodeId
    }

    if(nodeId == 0) {                                               // Node 0 must reach out to middleware
        if(reachMiddleware(machines) != 1) {                        // Reach middleware step goes to every node to distribute config file
            printf("Attempt to reach out to middleware failed");    // Alternatively could distribute config file earlier at middleware comm level (makes more sense)
        }
        next_node = connect(machines[1].ip, machines[1].port);
        sendConfig(next_node, machines, configLen);
        prev_node = listenAccept(59000);
    } else {
        prev_node = listenAccept(59000 + nodeId);
        machines = recConfig(prev_node);
        if(nodeId+1 < lenConfig){
            next_node = connect(machines[nodeId+1].ip, machines[nodeId+1].port);
        } else {
            next_node = connect(machines[0].ip, machines[0].port);
        }
    }



}

// User Function
int starSetup( /* Args? */ ) {

    struct Config * nodes;  // Struct for storing the config file information

    if(configPath != "") {
        nodes = readConfig(configPath);
        // Determine nodeId
    }

    if (nodeID == 0) {
        int client_sockets[30];

        if(reachMiddleware(nodes) != 1) {  // Distribute config file to every node
            printf("Attempt to reach out to middleware failed");
        }

        // somehow wait until all clients are ready and waiting?

        client_nodes = starConnect(nodes, PORT);
    }
    else {
        int central_node;
        central_node = listenAccept(PORT);
    }

    // what is being returned? An socket array for central node and a socket for others?
}

// User Function
int fullyConnectedSetup( /* Args? */ ) {

}

// User Function
void Send(int nodeId, char * data, int dataLen) {

}

// User Function
char * Receive(int nodeId) {

}