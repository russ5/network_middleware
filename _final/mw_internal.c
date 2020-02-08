#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
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

void sendConfig(int bg_sock, char * configPath) {
    char buff[CONFIG_BUFF];
    FILE *fp_config;

    fp_config = fopen(configPath, "r");
    if (fp_config == NULL) {
        printf("File open error\n");
        return;
    }

    while (fgets(buff, CONFIG_BUFF, fp_config) != NULL) // fgets reads upto MAX character or EOF
        write(bg_sock, buff, sizeof(buff));  // sent the file data to stream

    /*
    while(1) {
        nread = fread(buff, 1, CONFIG_BUFF, fp_config);
        //printf("Bytes read: %d\n", nread);
        if(nread > 0) {
            send(bg_sock, buff, nread, 0);
        }
        if(nread < CONFIG_BUFF)
            break;
    }
    */
    fclose(fp_config);
    //printf("sendConfig: Needs to be polished\n");
}

/// Change this to save config to temp directory seems better
void recConfig(int sockId) {
    char buff[CONFIG_BUFF];
    FILE * fp_config;

    fp_config = fopen("/tmp/config.txt", "w");
    if(fp_config == NULL) {
        printf("Error opening/creating file\n");
        return;
    }

    while(read(sockId, buff, CONFIG_BUFF) > 0)
        fprintf(fp_config, "%s", buff);

    fclose(fp_config);
    return;
    /*
    while((bytesReceived = read(sockId, buff, CONFIG_BUFF)) > 0)
    {
        printf("Bytes received %d\n",bytesReceived);
        fwrite(buff, 1, bytesReceived, fp_config);
    }
    */


    //printf("recConfig: needs to be polished\n");
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
            if(bg_sock < 0) {
                return -1;
            }
            /// send app name
            sprintf(buff, "%03d", strlen(progPath));             // Header of msg length
            strcat(buff, progPath);                              // Add msg onto end of buffer
            send(bg_sock, buff, strlen(progPath)+HEADER_LEN, 0);
            /// Send Config
            //sendConfig(bg_sock, configPath);                   // May change args for this func
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
            sendConfig(bg_sock, configPath);               // May change args for this func
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

int * fullConnect(struct Config * machines, int nodeId, int numOfMachines){
    int client_sockets[numOfMachines - 1 - nodeId];
    for(int i = nodeId + 1; i < numOfMachines; i++){
        client_sockets[i - nodeId - 1] = Connect(machines[i].ip, machines[i].port);
    }
    return client_sockets;
}

int * fullConnectListenAccept(int port, int nodeNum){
    int opt = 1;
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
    if (listen(listening_socket, nodeNum) < 0)
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
        for (int i = 0; i < nodeNum; i++) {
            //socket descriptor
            sd = client_sockets[i];

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
            for (int i = 0; i < nodeNum; i++) {
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

int listenAccept(int port, int * sockIds, int flag) {
    int sockfd, server_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int enable = 1;
    int i = 0;

// Set up server and listen
    if(flag == 0 || flag == 1) {
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
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
        if(flag == 1) {
            sockIds[1] = sockfd;
        }
    } else {
        sockfd = sockIds[1];                // listening sock already set up (avoid the dual bind)
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

    sockIds[0] = server_sock;               // Assign important sock ID (used for communication)

    if(flag == 0) {
        close(sockfd);
    }
    return 1;
}