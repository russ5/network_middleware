#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../mw_public.h"

void main(int argc, char const *argv[]) {
    printf("Main Loop");
    int node = atoi(argv[1]);
    char * msg = "Testing the elevated connection scheme";
    char * buffer;
    int * sockIds;

    if(node == 0){
        printf("Start Ring Setup");
        sockIds = ringSetup(node, "ringTestConfig.txt");
        Send(1, msg, strlen(msg), sockIds);
    } else {
        sockIds = ringSetup(node, "");
        buffer = Receive(node-1, sockIds);
        Send(node+1, buffer, 512, sockIds);
    }
    close(sockIds[node+1]);
}