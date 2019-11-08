#include "_middleware.h"

int * ringSetup(char * progName) {
    //char *fileName = "config.txt";
    int i = 0;
    int enable = 1;
    //FILE *fp = fopen(fileName, "r");
    int num_nodes;
    char str_num_nodes[32];
    char inst[1024];
    char ip[16];
    struct Config conf_ip[64];

    // Server-end vars
    int sockfd, server_sock, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    //char inst_buff[4] = {0};

    // Client-end setup vars
    int client_sock = 0;
    struct sockaddr_in serv_addr;

    static int sock_num[2];

    //fgets(str_num_nodes, 32, fp);
    //strtok(str_num_nodes, "\n");
    num_nodes = 4;
    /*
    for (i = 0; i < num_nodes; i++) {
        fgets(conf_ip[i].ip, MAX_IP, fp);
        strtok(conf_ip[i].ip, "\n");
    }
    */
// Reuse socket numbers (for looping)
    //setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int))
// Connect to node 1 (client to server)
    if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    //setsockopt(client_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT + 1);
    strcpy(ip, conf_ip[0].ip);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(client_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    //printf("Connected");
    /*
// Send config file info to continue connecting
    send(client_sock, str_num_nodes, strlen(str_num_nodes), 0);      // Send num of nodes
    for (i = 1; i < num_nodes; i++) {
        read(client_sock, buffer, 1);       // Not sure if this read is required to maintain timing
        send(client_sock, conf_ip[i].ip, strlen(conf_ip[i].ip), 0);
    }
    */
// Set up server and listen (once connected ring is complete)
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return -1;
        //exit(EXIT_FAILURE);
    }
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    //setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    // Bind socket
    if (bind(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        return -1;
        //exit(EXIT_FAILURE);
    }
// Wait for node prev node to reach out
    if (listen(sockfd, 3) < 0) {
        perror("listen");
        return -1;
        //exit(EXIT_FAILURE);
    }
// Accept connection
    if ((server_sock = accept(sockfd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
        perror("accept");
        return -1;
        //exit(EXIT_FAILURE);
    }
    printf("Ring Setup Complete!");
    sock_num[0] = client_sock;
    sock_num[1] = server_sock;
// Send app to launch
    send(client_sock, progName, strlen(progName), 0);

    return sock_num;
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
