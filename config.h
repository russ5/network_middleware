#ifndef CAPSTONE_CONFIG_H
#define CAPSTONE_CONFIG_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAXCHAR 1000

struct machine {
    int port;
    char ip_address[30];
};

int* get_topology(char* filename);

struct machine* get_machines(char* filename);


#endif //CAPSTONE_CONFIG_H
