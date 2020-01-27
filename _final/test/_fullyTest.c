#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../mw_public.h"

void main(int argc, char const *argv[]) {
    printf("Main Loop\n");
    int port = atoi(argv[1]);
    char * msg = "Test message";
    int * sockIds;
    sockIds = fullyConnectedSetup("./ringTestConfig.txt", port);
//    if(port == 59000){
//        printf("Running node 0\n");
//        Send(1, msg, strlen(msg), sockIds);
//        for(int i = 1; i < 4; i++) {
//            close(sockIds[i])
//        }
//    }
//    else {
//
//    }
}


