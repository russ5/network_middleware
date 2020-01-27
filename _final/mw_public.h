#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#define BUFFER 512

/*
 * Notes:
 *  - Axing connections between app and middleware in setup?
 *  - Need some control of Sends and Receives for data larger than BUFFER
 *  - Port control for operating on same machine (same IP, different ports)
 *
 *  - Need to setup the middleware app functionality
 */

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
int ringSetup(int comIds[], int nodeId, char * configPath); // Include config path here?

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
int starSetup( /* Args? */ );

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
int * fullyConnectedSetup(char * configPath, int port);

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
void Send(int nodeId, char * data, int dataLen, int * sockIds);

/** #FUNCTION# =========================================================================================================
 *  Name ..........:
 *  Description ...:
 *  Parameters ....:
 *  Return values .:
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
int Receive(char * buffer, int nodeId, int * sockIds);

#endif
