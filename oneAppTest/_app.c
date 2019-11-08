#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "_middleware.h"

/*
 * Ask Manjikian:
 *      - Port nums don't need to be different when run on multiple machines (simplifies background.exe)
 *      - UDP would be more flexible but is much more work at this stage
 *      - Setting up more flexible connection schemes is more difficult with this setup (need to tell background processes to open more clients if needed)
 */

int main(int argc, char const *argv[]) {
    int empty[2] = {0, 0};
    int * sock_ids = empty;
    int node = 0;
    char buff[1024];

    if(argv[1] != 0){
        node = atoi(argv[1]);
        //printf("%d", node);
    }

    if(node == 0) {
        printf("This is a test app\n");
        // Establish connections
        sock_ids = ringSetup("./app");
    } else {    // Not node 0, fork-exec from background app
        node = atoi(argv[1]);
        sock_ids[0] = atoi(argv[2]);
        sock_ids[1] = atoi(argv[3]);
        printf("Hello from node %d\n", node);
    }
    close(sock_ids[0]);
    close(sock_ids[1]);
    return 1;
}