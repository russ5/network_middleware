#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "mw_internal.h"

struct Config * readConfig(char * configPath) {
    FILE *fp;
    char str[MAXCHAR];
    fp = fopen(configPath, "r");
    if (fp == NULL){
        printf("Could not open file %s\n",configPath);
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
                    strcpy(machines[line_count - 1].ip, pt);
                    is_it_ip = 0;
                }
                else if(is_it_ip == 0){
                    machines[line_count - 1].port = atoi(pt);
                }
                pt = strtok(NULL, ",");
            }
            machines[line_count - 1].nodeID = line_count - 1;
        }
        line_count++;
    }
    fclose(fp);
    return machines;
}

int getNumOfMachines(char * configPath){
    FILE *fp;
    char str[MAXCHAR];
    fp = fopen(configPath, "r");
    if (fp == NULL){
        printf("Could not open file %s",configPath);
        exit(EXIT_FAILURE);
    }
    int net_top;
    int line_count = 0;
    while (fgets(str, MAXCHAR, fp) != NULL) {
        line_count++;
    }
    fclose(fp);
    return --line_count;
}

int checkIPMatch(char *ipAddress){
    // Get IP Address of current machine
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    checkHostName(hostname);
    host_entry = gethostbyname(hostbuffer);
    checkHostEntry(host_entry);
    IPbuffer = inet_ntoa(*((struct in_addr*)
            host_entry->h_addr_list[0]));
    if(strcmp(ipAddress,IPbuffer) == 0){
        return 1;
    }
    else{
        return 0;
    }
}

// Returns hostname for the local computer
void checkHostName(int hostname)
{
    if (hostname == -1)
    {
        perror("gethostname");
        exit(1);
    }
}

// Returns host information corresponding to host name
void checkHostEntry(struct hostent * hostentry)
{
    if (hostentry == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }
}

// Converts space-delimited IPv4 addresses
// to dotted-decimal format
void checkIPbuffer(char *IPbuffer)
{
    if (NULL == IPbuffer)
    {
        perror("inet_ntoa");
        exit(1);
    }
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

void sendConfig(int port, char * ip, char * configPath) {
    printf("sendConfig: Needs to be polished\n");
}

/// Change this to save config to temp directory seems better
void recConfig(int sockId) {
    printf("recConfig: needs to be polished\n");
}

int reachMiddleware(struct Config * machines, char * configPath, char * progPath) {
    // Iterate over every node
    // connect to each node read from config
    // send app to launch & node ID
    // close connection
    int localTest = 1;

    int i = 0;
    int port;
    int bg_sock;        // background socket
    char * ip;
    char buff[128];

    if(localTest) {
        for(i=0; i<3; i++) {        // Iterate over two background apps
            ip = machines[i].ip;    // Local machine
            port = 58901+i;
            bg_sock = Connect(ip, port);
            /// send app name
            sprintf(buff, "%03d", strlen(progPath));             // Header of msg length
            strcat(buff, progPath);                              // Add msg onto end of buffer
            send(bg_sock, buff, 128, 0);
            /// Send Config
            //sendConfig(port, ip, configPath);                   // May change args for this func
            /// Close the socket
            close(bg_sock);
        }
    } else {
        for (i = 0; i++; i < 3) {                /// NEEDS TO BE FIXED (SHOULDN'T BE HARD CODED)
            port = PORT_BG;
            ip = machines[i].ip;
            bg_sock = Connect(ip, port);
            /// send app name
            sprintf(buff, "%03d", strlen(progPath));             // Header of msg length
            strcat(buff, progPath);                              // Add msg onto end of buffer
            send(bg_sock, progPath, 128, 0);
            /// Send Config
            sendConfig(port, ip, configPath);               // May change args for this func
            /// Close the socket
            close(bg_sock);
        }
    }
    return 1;
}

int Connect(char * ip, int port) {
    int client_sock = 0;
    struct sockaddr_in serv_addr;

    if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

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

int * starConnect (struct Config * nodes) {
    int client_socket[30];

    // Hardcoded loop to add clients
    for (int i = 1; i < 4; i++) {
         client_socket[i-1] = connect(nodes[i].ip, nodes[i].port); // add clients to array
    }

    return client_socket;
}

int * fullConnect(struct Config * machines, int nodeId, int numOfMachines){
    int client_sockets[numOfMachines - 1 - nodeId];
    for(int i = nodeId + 1; i < numOfMachines; i++){
        client_sockets[i - nodeId - 1] = connect(machines[i].ip, machines[i].port);
    }
    return client_sockets;
}

int * fullConnectListenAccept(int port, int nodeNum){
    int opt = TRUE;
    int listening_socket , addrlen , new_socket , client_sockets[nodeNum + 1], activity, valread , sd;
    int max_sd;
    struct sockaddr_in address;

    fd_set readfds;

    // Initialise all client_sockets[] to 0.
    //  # of client_sockets[]s is the same as node num
    for (int i = 0; i < nodeNum; i++)
    {
        client_sockets[i] = 0;
    }

    // Listening socket
    if( (listening_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if( setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(listening_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", port);

    // Queue can be same size as the node num
    if (listen(listening_socket, node_num) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
    // Wait for more connections until there are as many numOfConnections as the nodeId
    int numOfConnections = 0;
    while(numOfConnections < nodeNum) {
        //clear the socket set
        FD_ZERO(&readfds);
        //add master socket to set
        FD_SET(listening_socket, &readfds);
        max_sd = listening_socket;

        //add child sockets to set
        for (i = 0; i < node_num; i++) {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if (sd > 0)
                FD_SET(sd, &readfds);

            //highest file descriptor number, need it for the select function
            if (sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            printf("select error");
        }

        //If something happened on the master socket ,
        //then its an incoming connection
        if (FD_ISSET(listening_socket, &readfds)) {
            if ((new_socket = accept(listening_socket,
                                     (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            // Increment num of connections
            numOfConnections = numOfConnections + 1;
            //inform user of connection information
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket,
                   inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            //add new socket to array of sockets
            for (i = 0; i < node_num; i++) {
                //if position is empty
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);

                    break;
                }
            }
        }
    }
    return client_sockets;
}
int listenAccept(int port) {
    int sockfd, server_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int i = 0;

// Set up server and listen
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    int enable = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

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