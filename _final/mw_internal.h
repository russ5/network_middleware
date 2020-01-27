#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#define MAXCHAR 64
#define MAX_IP_LEN 30
#define PORT 59000
#define PORT_BG 58900
#define BUFFER 512
#define CONFIG_BUFF 2048
#define MAX_IP 24

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
    int nodeID;
};

/***** Functions *****/

/** #FUNCTION# =========================================================================================================
 *  Name ..........: readConfig
 *  Description ...: read the configuration file
 *  Parameters ....: char *configPath
 *  Return values .: struct Config* (array of port, IP and nodeID)
 *  Modified ......: Changed machine struct to Config struct
 *  Remarks .......: See Tae for info about functionality
 *                   Has been modified slightly for port into mw_internal so may not work
 *  Related .......:
**  ================================================================================================================= */
struct Config * readConfig(char * configPath);                              /// Finished

/** #FUNCTION# =========================================================================================================
 *  Name ..........: getNumOfMachines
 *  Description ...: read the configuration file and returns number of machines in network
 *  Parameters ....: char *configPath
 *  Return values .: int that indicates number of machines
 *  Modified ......:
 *  Remarks .......: See Tae for info about functionality
 *                   Has been modified slightly for port into mw_internal so may not work
 *  Related .......:
**  ================================================================================================================= */
int getNumOfMachines(char * configPath);                              /// Finished

/** #FUNCTION# =========================================================================================================
 *  Name ..........: checkIPMatch
 *  Description ...: Checks if inputted IP matches the current machine's IP
 *  Parameters ....: char *ipAddress (IP Address string)
 *  Return values .: int 1 == True, 0 == False
 *  Modified ......:
 *  Remarks .......: See Tae for info about functionality
 *                   Has been modified slightly for port into mw_internal so may not work
 *  Related .......: checkHostName, checkHostEntry, checkIPbuffer are all helper functions for this function
**  ================================================================================================================= */
int checkIPMatch(char *ipAddress);                                    /// Finished

void checkHostName(int hostname);                                     /// Finished

void checkHostEntry(struct hostent * hostentry);                      /// Finished

void checkIPbuffer(char *IPbuffer);

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
void launchProg(char * args[]);                                             /// Finished

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
void sendConfig(int port, char * ip, char * configPath);                    /// Have code externally

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
void recConfig(int sockId);                                                 /// Change to listen socket

/** #FUNCTION# =========================================================================================================
 *  Name ..........: reachMiddleware
 *  Description ...: Connect to each node's running middleware and launch the application
 *  Parameters ....: struct Config * machines, char * configPath, char * progPath
 *  Return values .:
 *  Modified ......:
 *  Remarks .......: progPath variable should start with "./"
 *  Related .......:
**  ================================================================================================================= */
int reachMiddleware(struct Config * machines, char * configPath, char * progPath);           ///

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
int Connect(char * ip, int port);                                           /// Finished

/** #FUNCTION# =========================================================================================================
 *  Name ..........: starConnect
 *  Description ...: loops through connect() to create an array of connections for the central star node
 *  Parameters ....: int * ips: array of clients IPs, int port: port of all client sockets
 *  Return values .: int * client_sock: array of all client sockets
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
int * starConnect(struct Config * nodes);

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
int listenAccept(int port);                                                 /// Finished

int * fullConnect(struct Config * machines, int nodeId, int numOfMachines);
#endif
