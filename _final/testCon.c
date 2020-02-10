#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "mw_internal.h"
#include "mw_public.h"

#define IP "192.168.0.2"

void main(int argc, char const *argv[]) {
    int i;
    int idCon;
    int idList[2];
    int numMachines, headerVal;
    char buff[128];
    char buff2[128];
    char * path = "/tmp/config.txt";
    char * testStr = "This is a test";
    char * appName = "test/ringTestConfig.txt";
    struct Config * machines;

    if(argc > 1) {
        listenAccept(59000, idList, 0);
        // Config Sending Receiving Test
        // recConfig(idList[0]);
        read(idList[0], buff, strlen(testStr));
        printf("%s\n", buff);
        close(idList[0]);



        /*///
        read(idList[0], buff, 3);                // Read the header
        buff[3] = '\0';                          // Manually add null terminator to app name length
        headerVal = atoi(buff);                  // Convert header to integer
        printf("%d\n", headerVal);
        read(idList[0], buff, headerVal);
        printf("%s\n", buff);

        printf("\n");
        //memset(buff, 0, 128);

        read(idList[0], buff, 3);                // Read the header
        buff[3] = '\0';                          // Manually add null terminator to app name length
        headerVal = atoi(buff);                  // Convert header to integer
        printf("%d\n", headerVal);
        read(idList[0], buff, headerVal);
        printf("%s\n", buff);
         */
    } else {
        idCon = Connect(IP, 59000);
        strcat(buff, testStr);
        send(idCon, buff, strlen(testStr), 0);
        /*
        sprintf(buff, "%03d", strlen(testStr));             // Header of msg length
        strcat(buff, testStr);                              // Add msg onto end of buffer
        send(idCon, buff, strlen(testStr) + 3, 0);          // Send first msg

        //memset(buff, 0, 128);

        sprintf(buff, "%03d", strlen(appName));             // Header of msg length
        strcat(buff, appName);                              // Add msg onto end of buffer
        send(idCon, buff, strlen(appName) + 3, 0);          // Send 2nd msg
        printf("%s\n", buff);
         */
        close(idCon);
    }
}