#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "_middleware.h"
#define MAX_IP 24
#define PORT 59000

int main(int argc, char const *argv[]) {
    int node = atoi(argv[1]);

    int i = 0, j = 0;
    int num_nodes = 4;
    char str_num_nodes[32];
    char inst[1024];
    char ip[16];
    struct Config conf_ip[64];

    // Server-end vars
    int sockfd, server_sock, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char inst_buff[4] = {0};

    // Client-end setup vars
    int client_sock = 0;
    struct sockaddr_in serv_addr;

    while(1) {
        if(j>0){
            printf("Background looped\n");
        }
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        int enable = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        //setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT + node);
        // Bind socket
        if (bind(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        // Wait for node prev node to reach out
        if (listen(sockfd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        // Accept connection
        if ((server_sock = accept(sockfd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        /*
        // Read remaining config
        read(server_sock, buffer, 32);
        strcpy(str_num_nodes, buffer);
        num_nodes = atoi(str_num_nodes);
        for (i = node; i < num_nodes; i++) {
            send(server_sock, "a", 1, 0);
            read(server_sock, buffer, MAX_IP);
            strcpy(conf_ip[i].ip, buffer);
        }
        */
        // Connect to next node in line
        if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
        }
        //setsockopt(client_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        //setsockopt(client_sock, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
        serv_addr.sin_family = AF_INET;
        if(node == num_nodes-1) {
            serv_addr.sin_port = htons(PORT);       // On last node, point back to node 0
        } else {
            serv_addr.sin_port = htons(PORT + node + 1);
        }
        //printf("%s\n", conf_ip[node].ip);
        if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }
        if (connect(client_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed \n");
            return -1;
        }
        /*
        // Send remainder of config OR reached node 0
        if(node != num_nodes-1) {
            send(client_sock, str_num_nodes, strlen(str_num_nodes), 0);      // Send num of nodes
            for (i = node+1; i < num_nodes; i++) {
                read(client_sock, buffer, 1);       // Not sure if this read is required to maintain timing
                send(client_sock, conf_ip[i].ip, strlen(conf_ip[i].ip), 0);
            }
        }
        */
        // Finished setup at this node next get app to launch
        read(server_sock, inst_buff, 1024);
        send(client_sock, inst_buff, 1024, 0);  // Send app to next in line
        // Construct args[]
        char _node[9];
        char _clientSock[9];
        char _serverSock[9];
        sprintf(_node, "%d", node);
        sprintf(_clientSock, "%d", client_sock);
        sprintf(_serverSock, "d", server_sock);
        char * args[] = {inst_buff, _node, _clientSock, _serverSock, NULL};
        launchProg(args);
        j = j + 1;
        close(sockfd);
        close(client_sock);
    }
}