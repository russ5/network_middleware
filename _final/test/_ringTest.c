#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../mw_public.h"

void main(int argc, char const *argv[]) {
    printf("Test launched\n");
    int node = atoi(argv[1]);
    char * msg = "Testing the elevated connection scheme\n";
    char buffer[BUFFER];
    int comIds[MAX_MACHINES];
    int numberOfNodes =

    if(node == 0){
        printf("Start Ring Setup\n");
        if(ringSetup(comIds, node, "ringTestConfig.txt") != 1) {
            printf("Ring setup failed\n");
            return;
        }
        //printf("Ring setup complete\n");
        Send(1, msg, strlen(msg), comIds);
        Receive(buffer, 3, comIds);
        printf("%s", buffer);
    } else {
        ringSetup(comIds, node, "test/ringTestConfig.txt");
        Receive(buffer, node-1, comIds);
        if(node == 3) {
            Send(0, buffer, BUFFER, comIds);
        } else {
            Send(node+1, buffer, BUFFER, comIds);
        }

        printf("%s", buffer);
    }
    close(comIds[node+1]);
}