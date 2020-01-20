#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "../mw_internal.h"
#include "../mw_public.h"

void main(int argc, char const *argv[]) {

    char * msg = "./test/testText";
    int id = Connect("127.0.0.1", 58901);
    char buff[64];
    char tmpBuff[32];
    int headerVal;
    int i;
    //struct Config * machines = readConfig("ringTestConfig.txt");

    //char *token;

    sprintf(buff, "%03d", strlen(msg));         // Header of msg length
    strcat(buff, msg);                          // Add msg onto end of buffer

    strncpy(tmpBuff, buff, 3);                  // Split header to read
    tmpBuff[3] = "\0";                          // Manually add null terminator
    headerVal = atoi(tmpBuff);                  // Convert header to integer
    strncpy(tmpBuff, buff+3, 3+headerVal);      // Strip out header (length)

    //printf("%s", tmpBuff);
    //char * args[] = {tmpBuff, NULL};
    //launchProg(args);

    send(id, buff, strlen(buff), 0);
    close(id);

    id = Connect("127.0.0.1", 59001);
    send(id, "Secondary connection", 21, 0);
    close(id);
}