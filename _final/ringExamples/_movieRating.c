#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../mw_public.h"

void main(int argc, char const *argv[]) {
    int node = atoi(argv[1]);
    char buffIn[BUFFER], buffOut[BUFFER], userIn[BUFFER];
    int comIds[MAX_MACHINES];

    if(node == 0) {
        if(ringSetup(comIds, node, "ringTestConfig.txt") != 1) {
            printf("Ring setup failed\n");
            return;
        }

        /// Get user input
        printf("Please Input a Media Material\n");
        fgets(userIn, BUFFER, stdin);
        printf("User entered: \"%s\"\n", userIn);

        /// Send to first Node
        Send(1, userIn, strlen(userIn), comIds);

        /// Get all Nodes' responses
        Receive(buffIn, 3, comIds);
        printf("%s\n", buffIn);
    } else {
        if(ringSetup(comIds, node, "test/ringTestConfig.txt") != 1) {
            printf("Ring setup failed\n");
            return;
        }
        Receive(buffIn, node-1, comIds);
        if(node<3) {

            Send(node+1, buffOut);
        } else {
            Send(0, );
        }
    }

    close(comIds[node+1]);
}