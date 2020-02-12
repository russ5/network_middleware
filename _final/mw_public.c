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

int getNumOfNodes(char * configPath) {
    FILE *fp;
    char str[MAXCHAR];
    fp = fopen(configPath, "r");
    if (fp == NULL){
        printf("Could not open file %s",configPath);
        exit(EXIT_FAILURE);
    }
    int net_top;
    int line_count = 0;
    while (fgets(str, MAXCHAR, fp) != NULL) {
        line_count++;
    }
    fclose(fp);
    return --line_count;
}

int getCurrNode(int argc, char const * argv[]) {
    int nodeNum = -1;
    if(argc == 1){
        nodeNum = 0;
    } else {
        nodeNum = atoi(argv[1]);
    }
    return nodeNum;
}

// User Function
int ringSetup(int comIds[], int nodeId, char * configPath, char const * argv[]) {  // Include config path here?
    struct Config * machines;                   // Struct for storing the config file
    int sockIds[2];
    int numMachines = 0;
    //char const * appPath = argv[0];
    // Loop variables
    int i;

    if(nodeId == 0) {                                               // Node 0 must reach out to middleware
        machines = readConfig(configPath);
        numMachines = getNumOfMachines(configPath);
        //printf("%d\n", numMachines);
        if(reachMiddleware(machines, configPath, argv[0]) != 1) {            // Reach middleware step goes to every node to distribute config file
            printf("Attempt to reach out to middleware failed\n");
            return -1;
        }
        //printf("%d\n", machines[nodeId].port);
        comIds[nodeId+1] = Connect(machines[1].ip, machines[1].port);
        listenAccept(machines[nodeId].port, sockIds, 0);
        comIds[numMachines-1] = sockIds[0];
        //printf("Made it full circle\n");
    } else {
        // Read the config
        machines = readConfig(configPath);                         /// Will need to change to tmp directory
        numMachines = getNumOfMachines(configPath);                /// Will need to change to tmp directory
        // Wait for previous node to reach out
        //printf("%d\n", machines[nodeId].port);
        listenAccept(machines[nodeId].port, sockIds, 0);
        comIds[nodeId-1] = sockIds[0];
        if(nodeId+1 < numMachines){                               
            comIds[nodeId+1] = Connect(machines[nodeId+1].ip, machines[nodeId+1].port);
        } else {
            //printf("Node 3 connection\n");
            //printf("%s, %d\n", machines[0].ip, machines[0].port);
            comIds[0] = Connect(machines[0].ip, machines[0].port);
        }
    }
    //int * ptr = comIds;
    return 1;
}

// User Function
int starSetup(int clientNodes[], int nodeId, char * configPath) {

    struct Config * nodes;  // Struct for storing the config file information
    char * appPath = "./test/starTest";
    int sockIds[2];

    if(configPath != "") {
        nodes = readConfig(configPath);
    }

    if (nodeId == 0) {

        if(reachMiddleware(nodes, configPath, appPath) != 1) {  // Reach out to other nodes and launch app
            printf("Attempt to reach out to middleware failed");
        }

        for (int i = 1; i < 4; i++) {
            printf("Connecting to node %d, ip %s, port %d\n", i, nodes[i].ip, nodes[i].port);
            clientNodes[i-1] = Connect(nodes[i].ip, nodes[i].port); // add clients to array
        }
    }
    else {
        printf("Listening at port %d\n", (PORT + nodeId));
        listenAccept(nodes[nodeId].port, sockIds, 1);
        clientNodes[0] = sockIds[0];
        printf("Connection made: %d\n", clientNodes[0]);
    }

    return 1;
}

// User Function
int * fullyConnectedSetup(char * configPath, int port) {
    struct Config * machines = readConfig(configPath);                   // Struct for storing the config file
    char * appPath = "./test/fulltest";
    int numOfMachines = getNumOfMachines(configPath);
    int nodeId;
    int i;
    // Identify self in config
    for(i = 0; i < numOfMachines; i++){
        if(checkIPMatch(machines[i].ip) && machines[i].port == port){
            nodeId = i;
            break;
        }
        if( i + 1 == numOfMachines){
            perror("Error finding machine in config file");
            exit(EXIT_FAILURE);
        }
    }
    // Node 0 must reach out to other machines
    if(nodeId == 0){
        if(reachMiddleware(machines, configPath, appPath) != 1) {  // Distribute config file to every node
            printf("Attempt to reach out to middleware failed");
        }
        return fullConnect(machines, nodeId, numOfMachines);
    }
    else{
        int * inbound_sockets = fullConnectListenAccept(machines[nodeId].port, nodeId);
        int * outbound_sockets = fullConnect(machines, nodeId, numOfMachines);
        // As many connections as there are machines minus self
        int nodes[numOfMachines - 1];
        // Combine inbound and outbound sockets into a single array
        memcpy(nodes, inbound_sockets, nodeId * sizeof(int));
        memcpy(nodes + nodeId, outbound_sockets, (numOfMachines - 1 - nodeId) * sizeof(int));
        return nodes;
    }
}

// User Function
int Send(int nodeDest, char * data, int dataLen, int * sockIds) {
    /// Need loops to ensure all of the data is sent
    send(sockIds[nodeDest], data, BUFFER, 0);
    return 1;
}

// User Function
int Receive(char * buffer, int nodeId, int * sockIds) {
    //char buff[BUFFER];
    read(sockIds[nodeId], buffer, BUFFER);
    //char * ptr = buff;
    return 1;
}