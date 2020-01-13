#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "mw_public.h"
#include "mw_internal.h"

#define MAX_IP 24
#define PORT 59000

int main(int argc, char const *argv[]) {
    int node = atoi(argv[1]);

    int i = 0, j = 0;
    int num_nodes;
    char str_num_nodes[32];
    char inst[1024];
    char ip[16];
    struct Config conf_ip[64];

    int localTest = argv[1];

    listenAccept(PORT + localTest);
    /// Receive app name

    /// Receive config file

    /// Save config file (/tmp/config.txt)

    /// Close connection

    // Fork exec new prog (Pass the node ID?)
    launchProg(args);



}