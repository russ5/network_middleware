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
int * ringSetup(int nodeId, char * configPath) {  // Include config path here?
    struct Config * machines;                   // Struct for storing the config file
    int next_node, prev_node;                   // Track the node-1 and the node+1 from this node
    int comIds[64];                             // Store the socket IDs for other nodes
    // Loop variables
    int i;

    if(configPath != "") {
       machines = readConfig(configPath);
       // Determine nodeId
    }

    if(nodeId == 0) {                                               // Node 0 must reach out to middleware
        if(reachMiddleware(machines, configPath) != 1) {            // Reach middleware step goes to every node to distribute config file
            printf("Attempt to reach out to middleware failed");    // Alternatively could distribute config file earlier at middleware comm level (makes more sense)
        }
        comIds[nodeId+1] = Connect(machines[1].ip, machines[1].port);
        //sendConfig(next_node, machines, configLen);
        comIds[3] = listenAccept(machines[nodeId].port);                            /// Needs logic (number of nodes would do)
    } else {
        // Read the config
        machines = readConfig(configPath);                         /// Will need to change to tmp directory
        // Wait for previous node to reach out
        comIds[nodeId-1] = listenAccept(machines[nodeId].port);
        if(nodeId+1 < 4){                                         /// Need to remove hard coding
            comIds[nodeId+1] = Connect(machines[nodeId+1].ip, machines[nodeId+1].port);
        } else {
            comIds[0] = Connect(machines[0].ip, machines[0].port);
        }
    }
    int * ptr = comIds;
    return ptr;
}

// User Function
int * starSetup(int nodeId, char * configPath) {

    struct Config * nodes;  // Struct for storing the config file information

    if(configPath != "") {
        nodes = readConfig(configPath);
        // Determine nodeId
    }

    if (nodeID == 0) {
        int client_nodes[30];

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

    return client_nodes;
}

// User Function
int fullyConnectedSetup( /* Args? */ ) {

}

// User Function
void Send(int nodeId, char * data, int dataLen, int * sockIds) {
    /// Need loops to ensure all of the data is sent
    send(sockIds[nodeId], data, BUFFER, 0);
}

// User Function
char * Receive(int nodeId, int * sockIds) {
    char buff[BUFFER];
    read(sockIds[nodeId], buff, BUFFER);
    char * ptr = buff;
    return ptr;
}