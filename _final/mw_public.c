#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "mw_internal.h"
#include "mw_public.h"

struct Config * readConfig(char * configPath) {
    FILE *fp;
    char str[MAXCHAR];
    fp = fopen(configPath, "r");
    if (fp == NULL){
        printf("Could not open file %s\n",configPath);
        exit(EXIT_FAILURE);
    }

    static struct Config* machines;
    machines = malloc((sizeof(int) + sizeof(char) * 30) * 15);
    int line_count = 0;
    char *pt;
    while (fgets(str, MAXCHAR, fp) != NULL) {
        if (line_count > 0) {
            pt = strtok(str, ",");
            int is_it_ip = 1;
            while(pt != NULL){
                if(is_it_ip == 1){
                    strcpy(machines[line_count - 1].ip, pt);
                    is_it_ip = 0;
                }
                else if(is_it_ip == 0){
                    machines[line_count - 1].port = atoi(pt);
                }
                pt = strtok(NULL, ",");
            }
            machines[line_count - 1].nodeID = line_count - 1;
        }
        line_count++;
    }
    fclose(fp);
    return machines;
}

int getNumOfMachines(char * configPath){
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

int getCurrNode(int numOfMachines, struct Config * machines){
    int nodeNum = -1;
    for(int i = 0; i < numOfMachines; i++){
        if(checkIPMatch(machines[i].ip) == 1){
            nodeNum = i;
        }
    }
    return nodeNum;
}

int * starSetup(char * configPath, char * appPath){
    int port = 59000;
    struct Config * machines = readConfig(configPath);                   // Struct for storing the config file
    int numOfMachines = getNumOfMachines(configPath);
    int nodeNum = getCurrNode(numOfMachines, machines);
    if(nodeNum == -1){
        printf("Couldnt find Node in config file\n");
    }
        // Node 0 must reach out to other machines
    else if(nodeNum == 0){
        printf("This is node 0 and reaching out\n");
        if(reachMiddleware(machines, appPath, numOfMachines) != 1) {  // Distribute config file to every node
            printf("Attempt to reach out to middleware failed");
        }
        return fullConnect(machines, nodeNum, numOfMachines);
    }
    else{
        return fullConnectListenAccept(machines[nodeNum].port, nodeNum, 0);
    }

}

int * fullyConnectedSetup(char * configPath, char * appPath) {
    int port = 59000;
    struct Config * machines = readConfig(configPath);                   // Struct for storing the config file
    int numOfMachines = getNumOfMachines(configPath);
    int nodeNum = getCurrNode(numOfMachines, machines);
    if(nodeNum == -1){
        printf("Couldnt find Node in config file\n");
    }
    // Node 0 must reach out to other machines
    else if(nodeNum == 0){
        printf("This is node 0 and reaching out\n");
        if(reachMiddleware(machines, appPath, numOfMachines) != 1) {  // Distribute config file to every node
            printf("Attempt to reach out to middleware failed");
        }
        return fullConnect(machines, nodeNum, numOfMachines);
    }
    else{
        int * inbound_sockets = fullConnectListenAccept(machines[nodeNum].port, nodeNum, 0);
        int * outbound_sockets = fullConnect(machines, nodeNum, numOfMachines);
        // As many connections as there are machines minus self
        int * nodes = (int*)malloc(numOfMachines * sizeof(int));
        // Combine inbound and outbound sockets into a single array
        //memcpy(where in nodes, what are you copying, how long is what is being copied)
        memcpy(nodes, inbound_sockets, nodeNum * sizeof(int));
        memcpy(nodes + nodeNum, outbound_sockets, (numOfMachines - 1 - nodeNum) * sizeof(int));
        return nodes;
    }
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