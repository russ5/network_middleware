#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../mw_public.h"

void main(int argc, char const *argv[]) {
    int node = atoi(argv[1]);
    char * msg = "Hello from the central node!\n";
    char buffer[BUFFER];
    int * sockIds = (int *) malloc(4* sizeof(int));
    int centralSock;

    if(node == 0) {
        starSetup(sockIds, node, "starTestConfig.txt");

        for(int i = 0; i < 3; i++)
            printf("Node %d - addr: 0x%08X, val: %d\n", i+1, &sockIds[i], sockIds[i]);

        for (int i = 0; i < 3; i++) {
            if (sockIds[i] == -1) {
                printf("Error: no connection at node %d\n", i);
            }
            else {
                printf("Sending msg to node %d\n", i);
                Send(i, msg, BUFFER, sockIds);
            }
        }

        for (int i = 0; i < 3; i++) {
            printf("Closing connection to node %d\n", i);
            close(sockIds[i]);
        }
    }
    else {
        // Not sure what needs changing here for star
        starSetup(sockIds, node, "starTestConfig.txt");
        Receive(buffer, 0, sockIds);
        printf("Receive completed\n");
        printf("Central node: %s", buffer);
        close(centralSock);
    }

}