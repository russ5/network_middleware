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
int ringSetup(int comIds[], int nodeId, char * configPath) {  // Include config path here?
    struct Config * machines;                   // Struct for storing the config file
    int sockIds[2];
    char * appPath = "./test/ringTest";
    // Loop variables
    int i;

    if(nodeId == 0) {                                               // Node 0 must reach out to middleware
        machines = readConfig(configPath);
        if(reachMiddleware(machines, configPath, appPath) != 1) {            // Reach middleware step goes to every node to distribute config file
            printf("Attempt to reach out to middleware failed");
        }
        comIds[nodeId+1] = Connect(machines[1].ip, machines[1].port);
        listenAccept(machines[nodeId].port, sockIds, 0);
        comIds[3] = sockIds[0];                                                /// Needs logic (number of nodes would do)
        printf("Made it full circle\n");
    } else {
        // Read the config
        machines = readConfig(configPath);                         /// Will need to change to tmp directory
        // Wait for previous node to reach out
        listenAccept(machines[nodeId].port, sockIds, 0);
        comIds[nodeId-1] = sockIds[0];
        if(nodeId+1 < 4){                                         /// Need to remove hard coding
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
int * starSetup(int nodeId, char * configPath) {

    struct Config * nodes;  // Struct for storing the config file information
    char * appPath = "./test/ringTest";
    int sockIds[2];

    if(configPath != "") {
        nodes = readConfig(configPath);
        // Determine nodeId
    }

    if (nodeId == 0) {
        int * client_nodes;

        if(reachMiddleware(nodes, configPath, appPath) != 1) {  // Distribute config file to every node
            printf("Attempt to reach out to middleware failed");
        }

        // somehow wait until all clients are ready and waiting?

        client_nodes = starConnect(nodes);
        return client_nodes;
    }
    else {
        listenAccept(PORT, sockIds, 0);
        int * central_node = sockIds[0];
        return central_node;
    }
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
int Receive(char * buffer, int nodeId, int * sockIds) {
    //char buff[BUFFER];
    read(sockIds[nodeId], buffer, BUFFER);
    //char * ptr = buff;
    return 1;
}