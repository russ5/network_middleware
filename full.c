#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "config.h"

#define TEXT_TO_SEND "Hello from node 0"

int main(int argc , char *argv[]){
    if(argc < 2){
        printf("Missing arguments. \n");
        exit(-1);
    }
    int* topology = get_topology("./config.txt");
    struct machine* machines = get_machines("./config.txt");
    int node_num = atoi(argv[1]);

    if(node_num == 0){
        printf("%i",topology[1]);
        for(int i = 1; i < topology[1]; i++) {
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
                    return -1;
                }
                if (connect(self_socket, (struct sockaddr *) &other_socket, sizeof(other_socket)) < 0) {
                    perror("Error connecting: ");
                    exit(-1);
                }

                in = write(self_socket,TEXT_TO_SEND,sizeof(TEXT_TO_SEND));
                if (in < 0){
                    perror("\nClient Error: Writing to Server");
                    exit(0);
                }

                while(1)
                {
                    in = read(self_socket,buff,1024);
                    if (in < 0)
                    {
                        perror("\nClient Error: Reading from Server");
                        return 0;
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

}
