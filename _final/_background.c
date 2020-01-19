#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "mw_internal.h"
#include "mw_public.h"

#define MAX_IP 24
#define PORT 59000

int main(int argc, char const *argv[]) {
    int node;

    int i = 0, j = 0;
    int nodeZero;
    int num_nodes;
    int lenAppName;
    char str_num_nodes[32];
    char inst[1024];
    char ip[16];
    //struct Config conf_ip[64];
    char _node[8];
    char *token;

    //int localTest = atoi(argv[1]);
    if(argc == 1) {
        node = 0;
    } else {
        node = atoi(argv[1]);
    }
    // Loop so app runs in background permanently
    while(1) {
        printf("Background launched at node %s\n", argv[1]);
        /// Wait for connection
        nodeZero = listenAccept(PORT + node);
        /// Receive app name
        read(nodeZero, inst, 1024);
        token = strtok(inst, "#");
        lenAppName = atoi(token);
        token = strtok(NULL, "#");
        /*
        while(token != NULL) {
            printf("\n%s", token);
            token = strtok(NULL, "#");
        }
         */
        char * tmp = inst;
        printf("%s", inst);
        /// Receive config file
        recConfig(nodeZero);
        /// Save config file (/tmp/config.txt)

        /// Close connection
        close(nodeZero);

        // Fork exec new prog (Pass the node ID?)
        sprintf(_node, "%d", node);
        char *args[] = {inst, _node, NULL};
        launchProg(args);
        printf("App finished\n");

    }
}