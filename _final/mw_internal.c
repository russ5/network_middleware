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

struct Config * readConfig(char * configPath) {
    FILE *fp;
    char str[MAXCHAR];
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        exit(EXIT_FAILURE);
    }

    static struct Config* machines;
    machines = malloc((sizeof(int) + sizeof(char) * 30) * 15);
    int line_count = 0;
    char *pt;
    while (fgets(str, MAXCHAR, fp) != NULL) {
        if (line_count > 0) {
            pt = strtok(str, ",");
            int is_it_ip = 1;
            while(pt != NULL){
                if(is_it_ip == 1){
                    strcpy(machines[line_count - 1].ip_address, pt);
                    is_it_ip = 0;
                }
                else if(is_it_ip == 0){
                    machines[line_count - 1].port = atoi(pt);
                }
                pt = strtok(NULL, ",");
            }
        }
        line_count++;
    }
    fclose(fp);
    return machines;
}

void launchProg(char * args[]) {
    pid_t childPid;
    childPid = fork();
    if (childPid == 0) {
        //printf("Fork successful");
        execvp(args[0], args);
    } else {
        int returnStatus;
        waitpid(childPid, &returnStatus, 0);
    }
}

void sendConfig(int sockId, char * pathConfig) {

}

/// Change this to save config to temp directory seems better
/*
struct Config * recConfig(int sockId) {
    struct Config * machines;

    return machines;
}
*/

int reachMiddleware(struct Config * machines) { // char * args[] here, or build it?
    // Iterate over every node
    // connect to each node read from config
    // send app to launch & node ID
    // close connection
    int i = 0;
    int port;
    char * ip;

    for(i=1; i++; i<4) {                /// NEEDS TO BE FIXED (SHOULDN'T BE HARD CODED)
        port = machines[i].port;
        ip = machines[i].ip;
        bg_sock = connect(ip, port);
        /// send app name
        /// send config file
        close(bg_sock);
    }
    return 1;
}

int connect(char * ip, int port) {
    int client_sock = 0;
    struct sockaddr_in serv_addr;

    if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons();
    //strcpy(ip, conf_ip[0].ip);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(client_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    return client_sock;
}

int * starConnect (int * ips, int port) {
    int client_socket[30];

    // Hardcoded loop to add clients
    for (int i = 1; i < 4; i++) {
        // client_socket[i-1] = connect(NODE_IP[i], PORT); // add clients to array
    }

    return client_socket;
}

int listenAccept(int port) {
    int sockfd, server_sock, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

// Set up server and listen
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    // Bind socket
    if (bind(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
// Wait for connect function call
    if (listen(sockfd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
// Accept connection
    if ((server_sock = accept(sockfd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    return server_sock;
}