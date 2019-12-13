#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "../include/fully_connected_connect.h"

#define TEXT_TO_SEND "boom"
#define TRUE 1

void initiate_connections(int node_num, int* topology, struct machine* machines){
    for(int i = node_num + 1; i < topology[1]; i++) {
        pid_t pid = fork();
        if(pid == 0){
            int self_socket, in;
            struct hostent *hp;
            char buff[1024] = {0};
            if((self_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
                perror("Socket failed");
                exit(-1);
            }
            printf("self_socket created successfully\n");
            struct sockaddr_in other_socket;
            other_socket.sin_family = AF_INET;
            other_socket.sin_port = htons(machines[i].port);
            // Convert IPv4 and IPv6 addresses from text to binary form
            if (inet_pton(AF_INET, machines[i].ip_address, &other_socket.sin_addr) <= 0) {
                printf("\nInvalid address/ Address not supported \n");
                exit(EXIT_FAILURE);
            }
            if (connect(self_socket, (struct sockaddr *) &other_socket, sizeof(other_socket)) < 0) {
                perror("Error connecting: ");
                exit(EXIT_FAILURE);
            }

            in = write(self_socket,TEXT_TO_SEND,sizeof(TEXT_TO_SEND));
            if (in < 0){
                perror("\nClient Error: Writing to Server");
                exit(EXIT_FAILURE);
            }

            while(TRUE)
            {
                in = read(self_socket,buff,1024);
                if (in < 0)
                {
                    perror("\nClient Error: Reading from Server");
                    exit(EXIT_FAILURE);
                }
                if(strncmp(buff,"end",3)==0)
                {
                    printf("Server disconnected");
                    break;
                }
                printf("\nMessage from other node: %s",buff);
            }
            close(self_socket);
            exit(0);
        }
    }
}