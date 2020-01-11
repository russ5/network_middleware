#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

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
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
void sendConfig(int sockId, struct Config * machines, int configLen);

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
struct Config * recConfig(int sockId);  /// This is changing to save config to tmp dir

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
int connect(char * ip, int port);

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
int listenAccept(int port);

// Incorporate Config reading code?
#endif
