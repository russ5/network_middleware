#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#define BUFFER 512
#define MAX_MACHINES 64

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
 *  Name ..........: Send
 *  Description ...: Send data to designated node
 *  Parameters ....: int nodeDest: The node to send data too
 *                   char * data: Pointer to the data to send
 *                   int dataLen: Length of data to send
 *                   int * sockIds: Pointer to array of socket IDs
 *  Return values .: int pass/fail
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
void Send(int nodeDest, char * data, int dataLen, int * sockIds);

/** #FUNCTION# =========================================================================================================
 *  Name ..........: Receive
 *  Description ...: The receiver function after sending data
 *  Parameters ....: char * buffer: Pointer to the buffer for data
 *                   int nodeId: The ID of the node that is sending
 *                   int * sockIds: The array of IDs previously generated
 *  Return values .: int pass/fail
 *  Modified ......:
 *  Remarks .......:
 *  Related .......:
**  ================================================================================================================= */
int Receive(char * buffer, int nodeId, int * sockIds);

#endif
