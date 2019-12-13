#include "../include/fully_connected.h"
#include "../include/config.h"
#include "../include/fully_connected_helpers.h"
#include "../include/fully_connected_listen.h"
#include "../include/fully_connected_connect.h"

int fully_connected(int port){
    // Get Config
    int* topology = get_topology("./config.txt");
    struct machine* machines = get_machines("./config.txt");
    int node_num;
    // Get node num using user inputted port number & the machine's ip address
    for(int i = 0; i < topology[1]; i++){
        if(checkIPMatch(machines[i].ip_address) && machines[i].port == port){
            node_num = i;
            break;
        }
        if( i + 1 == topology[1]){
            perror("Error finding machine in config file");
            exit(EXIT_FAILURE);
        }
    }
    // Open up listening port & initiate connections to other nodes based on node num
    initiate_listen(port, node_num);
    initiate_connections(node_num, topology, machines);

}