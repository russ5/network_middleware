#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define MAX_IP 16
#define PORT 59000

struct Config {
    char ip[MAX_IP];
};

int main(int argc, char const *argv[]) {
    int node = atoi(argv[1]);           // Convert arg to int

    char *fileName = "config.txt";
    int i = 0;
    FILE *fp = fopen(fileName, "r");
    int num_nodes;
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
    /*
    fgets(ip1, MAX_IP, fp);
    fgets(ip2, MAX_IP, fp);
    strtok(ip1, "\n");cd
    strtok(ip2, "\n");
    */
    if(node == 0){ // Treat this as node 0
        // Read the config file
        fgets(str_num_nodes, 32, fp);
        strtok(str_num_nodes, "\n");
        num_nodes = atoi(str_num_nodes);
        for(i=0; i<num_nodes; i++) {
            fgets(conf_ip[i].ip, MAX_IP, fp);
            strtok(conf_ip[i].ip, "\n");
        }
        // Connect to node 1 (client to server)
        if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT+1);
        strcpy(ip, conf_ip[0].ip);
        if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }
        if (connect(client_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed \n");
            return -1;
        }
        // Send config file info to continue connecting
        send(client_sock, str_num_nodes, strlen(str_num_nodes), 0);      // Send num of nodes
        for(i=1; i<num_nodes; i++) {
            read(client_sock, buffer, 1);       // Not sure if this read is required to maintain timing
            send(client_sock, conf_ip[i].ip, strlen(conf_ip[i].ip), 0);
        }
        // Set up server and listen (once connected ring is complete)
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        // Bind socket
        if (bind(sockfd, (struct sockaddr *)&address, sizeof(address))<0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        // Wait for node prev node to reach out
        if (listen(sockfd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        // Accept connection
        if ((server_sock = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        // Reached back to node 0 (ring complete)
        printf("Ring Setup Complete!\nEnter GO to test or EXIT to quit: ");
        fgets(inst, sizeof(inst), stdin);
        strtok(inst, "\n");
        printf("%s\n", inst);
        send(client_sock, inst, strlen(inst), 0);
        read(server_sock, inst_buff, 4);
        printf("Got back '%s' from node 3\n", inst_buff);
        printf("Goodbye\n");
    }
    else {
        // Create socket
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT + node );
        // Bind socket
        if (bind(sockfd, (struct sockaddr *)&address, sizeof(address))<0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        // Wait for node prev node to reach out
        if (listen(sockfd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        // Accept connection
        if ((server_sock = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        // Read remaining config
        read(server_sock, buffer, 32);
        strcpy(str_num_nodes, buffer);
        num_nodes = atoi(str_num_nodes);
        for(i=node; i<num_nodes; i++) {
            send(server_sock, "a", 1, 0);
            read(server_sock, buffer, MAX_IP);
            strcpy(conf_ip[i].ip, buffer);
        }
        // Connect to next node in line
        if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
        }
        serv_addr.sin_family = AF_INET;
        if(node == num_nodes-1) {
            serv_addr.sin_port = htons(PORT);       // On last node, point back to node 0
        } else {
            serv_addr.sin_port = htons(PORT + node + 1);
        }
        printf("%s\n", conf_ip[node].ip);
        if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }
        if (connect(client_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed \n");
            return -1;
        }
        // Send remainder of config OR reached node 0
        if(node != num_nodes-1) {
            send(client_sock, str_num_nodes, strlen(str_num_nodes), 0);      // Send num of nodes
            for (i = node+1; i < num_nodes; i++) {
                read(client_sock, buffer, 1);       // Not sure if this read is required to maintain timing
                send(client_sock, conf_ip[i].ip, strlen(conf_ip[i].ip), 0);
            }
        }
        // Finished setup at this node (wait for node 0 to initiate)
        read(server_sock, inst_buff, 4);

        printf("Testing node %d\n", node);
        printf("Goodbye\n");

        //printf("%s", buffer);
        send(client_sock, inst_buff, strlen(inst_buff), 0);
    }
}