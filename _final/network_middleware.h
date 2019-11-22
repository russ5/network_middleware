#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define MAX_IP 24
#define PORT 59000
#define BUFFER 512

/*
 * Notes:
 *  - Axing connections between app and middleware in setup?
 *  - Need some control of Sends and Receives for data larger than BUFFER
 *  - Port control for operating on same machine (same IP, different ports)
 *
 *  - Need to setup the middleware app functionality
 */
/***** Structures *****/
struct Config {
    char ip[MAX_IP];
};

/***** Functions *****/
struct Config readConfig(char * configPath);

void launchProg(char * args[]);

int reachMiddleware(); // char * args[] here, or build it?

int connect(char * ip, int sockId);

int listenAccept(char * ip, int sockId);

// User Function
int ringSetup(int nodeId, char * configPath); // Include config path here?

// User Function
int starSetup( /* Args? */ );

// User Function
int fullyConnectedSetup( /* Args? */ );

void Send(int nodeId, char * data);

char * Receive(int nodeId);

// Incorporate Config reading code?

