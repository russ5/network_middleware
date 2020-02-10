#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../mw_public.h"

// Test for multiple send and receives simultaneously to multiple nodes.
void main(int argc, char const *argv[]) {
    char * msgNode0 = "TEST MESSAGE\n";
    char buffer[BUFFER];
    int * sockIds;
    sockIds = fullyConnectedSetup("./config.txt","./fulltest");
    int numOfMachines = getNumOfMachines("./config.txt");
    struct config* machines = readConfig("./config.txt");
    int currNode = getCurrNode(numOfMachines, machines);
    printf("Current Node is: %i\n", currNode);
    printf("Num of Machines is: %i\n", numOfMachines);

}


