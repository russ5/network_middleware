#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define MAX_IP_LEN 30
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
    int port;
    char ip[MAX_IP_LEN];
};

/***** Functions *****/

/** #FUNCTION# =========================================================================================================
 *  Name ..........: readConfig
 *  Description ...: read the configuration file
 *  Parameters ....: char *configPath
 *  Return values .: struct Config* (array of port and IP)
 *  Modified ......: Changed machine struct to Config struct
 *  Remarks .......: See Tae for info about functionality
 *                   Has been modified slightly for port into mw_internal so may not work
 *  Related .......:
**  ================================================================================================================= */
struct Config * readConfig(char * configPath);

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
void launchProg(char * args[]);

/** #FUNCTION# =========================================================================================================
 *  Name ..........: reachMiddleware
 *  Description ...: Connect to each node's running middleware and launch the application
 *  Parameters ....: struct Config * machines
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
int reachMiddleware(struct Config * machines); // char * args[] here, or build it?

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
int connect(char * ip, int port, int sockId);

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
int listenAccept(char * ip, int port, int sockId);

// Incorporate Config reading code?
#endif
