#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../mw_public.h"

void main(int argc, char const *argv[]) {
    printf("Main Loop\n");
    int node = atoi(argv[1]);
    char * msg = "Testing the elevated connection scheme\n";
    char buffer[BUFFER];
    int comIds[64];

    if(node == 0){
        printf("Start Ring Setup\n");
        ringSetup(comIds, node, "ringTestConfig.txt");
        Send(1, msg, strlen(msg), comIds);
        Receive(buffer, 3, comIds);
        printf("%s", buffer);
    } else {
        ringSetup(comIds, node, "test/ringTestConfig.txt");
        Receive(buffer, node-1, comIds);
        if(node == 3) {
            Send(0, buffer, 512, comIds);
        } else {
            //printf("%d\n", comIds[node+1]);
            Send(node+1, buffer, 512, comIds);
        }

        printf("%s", buffer);
    }
    close(comIds[node+1]);
}