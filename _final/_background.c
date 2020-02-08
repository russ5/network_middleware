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

/** KNOWN ISSUES
 *      - Background wont loop (bind failed: Address already in use)
 */

int main(int argc, char const *argv[]) {
    int node;

    int i = 0, j = 0;
    int nodeZero;
    int num_nodes;
    int headerVal;
    int sockfd;
    int sockIds[2];
    char str_num_nodes[32];
    char inst[1024], tmpBuff[1024];
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
        if(j == 0) {
            listenAccept(PORT_BG + node, sockIds, 1);
        } else {
            listenAccept(PORT_BG + node, sockIds, 2);
        }
        /// Receive app name
        read(sockIds[0], inst, 3);                  // Read header
        inst[3] = '\0';                             // Manually add null terminator to header length
        headerVal = atoi(inst);                     // Convert header to integer
        read(sockIds[0], inst, headerVal);          // Read app name info
        printf("%s\n", inst);

        /// Receive config file
        if(node < 2) {
            //recConfig(nodeZero);
            //printf("Escaped recConfig\n");
        }
        /// Save config file (/tmp/config.txt)

        /// Close connection
        close(sockIds[0]);

        // Fork exec new prog (Pass the node ID?)
        if(node == 0) {                             ///
                                                    /// For operation on multiple PCs need to determine node ID
        }                                           ///
        sprintf(_node, "%d", node);
        char *args[] = {inst, _node, NULL};
        launchProg(args);
        printf("App finished\n");

        j = 1;
        //sleep(20);
    }
}