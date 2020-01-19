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

    char * msg = "./testText";
    int id = Connect("127.0.0.1", 59001);
    char buff[64];
    int tmp;
    char *token;

    sprintf(buff, "%03d", strlen(msg));
    strcat(buff, "#");
    tmp = atoi(buff);
    strcat(buff, msg);
    printf("%s", buff);

    send(id, buff, strlen(buff), 0);
    close(id);

}