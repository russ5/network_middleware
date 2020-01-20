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
        printf("Could not open file %s",configPath);
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
    //char * progName = "./ringTest";

    if(localTest) {
        for(i=0; i<3; i++) {        // Iterate over two background apps
            ip = "127.0.0.1";       // Local machine
            port = 58901+i;
            bg_sock = Connect(ip, port);
            printf("%s, %d\n", ip, port);
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
            port = machines[i].port;
            ip = machines[i].ip;
            //printf("%s, %d", ip, port);
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

int * starConnect (char * ip, int port) {
    int client_socket[30];

    // Hardcoded loop to add clients
    for (int i = 1; i < 4; i++) {
        // client_socket[i-1] = connect(NODE_IP[i], PORT); // add clients to array
    }

    return client_socket;
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
    /*
    while(bind(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        i++;
        if(i == 3) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        //sleep(3);
    }
    */
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