#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../mw_public.h"

void main(int argc, char const *argv[]) {
    printf("Main Loop");
    int node = atoi(argv[1]);
    char * msg = "Testing star connection";
    char * buffer;
    int * sockIds;
    int centralSock;

    if(node == 0) {
        printf("Start Star Setup");
        sockIds = starSetup(node, "starTestConfig.txt");
        Send(1, msg, strlen(msg), sockIds);

        for (int i = 1; i < 4; i++) {
            close(sockIds[i]);
        }
    }
    else {
        // Not sure what needs changing here for star
        centralSock = starSetup(node, "");
        buffer = Receive(0, sockIds);
        Send(0, buffer, 512, sockIds);
        close(centralSock);
    }

}