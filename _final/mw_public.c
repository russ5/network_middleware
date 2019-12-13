#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "mw_internal.h"
#include "mw_public.h"

// User Function
int ringSetup(char * configPath) {  // Include config path here?

}

// User Function
int starSetup(char * configPath) {
    // get set of socket descriptors
    // create a master socket on node 0
        // setsockopt to allow multiple connections
        // initiate setup on other machines
    // if not node 0, create a regular socket
    // bind socket to port
    // if master node, connect() to sockets
    // if regular node, accept() connection from master socket
    // once connected, communicate as necessary (using select() and send())
}

// User Function
int fullyConnectedSetup( /* Args? */ ) {

}

// User Function
void Send(int nodeId, char * data) {

}

// User Function
char * Receive(int nodeId) {

}