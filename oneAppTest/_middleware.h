#ifndef CAPSTONE_MW_H
#define CAPSTONE_MW_H
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

struct Config {
    char ip[MAX_IP];
};

int * ringSetup(char * progName);

void launchProg(char * args[]);

#endif //CAPSTONE_MW_H
