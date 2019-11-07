#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

/*
 First line of text file is an integer that corresponds to a network topology.
 0 = point to point (only two computers)
 1 = bus
 2 = ring
 3 = star
 4 = fully connected
 Any subsequent lines will hold the IP addresses & ports of the machines in the network. (Comma separated)
*/
int main() {
    int* topology = get_topology("./config.txt");
    printf("Index 0 contains the topology shape: %i\n", topology[0]);
    printf("Index 1 contains the number of machines in the network: %i\n", topology[1]);
    struct machine* machines = get_machines("./config.txt");
    printf("IP Address of first machine is: %s\n", machines[0].ip_address);
    printf("Port of first machine is: %i\n", machines[0].port);
}
