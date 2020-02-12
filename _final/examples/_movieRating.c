#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../mw_internal.h"
#include "../mw_public.h"

#define MAX_NODES 6

int getRating(char * title, int nodeId);

void main(int argc, char const *argv[]) {
    int node = getCurrNode(argc, argv);
    char buffIn[BUFFER], buffOut[BUFFER], userIn[BUFFER];
    int comIds[MAX_MACHINES];
    int numNodes = getNumOfNodes("movieRatingConfig.txt");
    int ratings[MAX_NODES];
    int i;

    if(node == 0) {
        if(ringSetup(comIds, node, "movieRatingConfig.txt", argv) != 1) {
            printf("Ring setup failed\n");
            return;
        }
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        //while(1) {
            /// Get user input
            printf("\nPlease Input a Movie Title\n");
            fgets(userIn, BUFFER, stdin);
            //printf("User entered: %s\n", userIn);

            /*if(strcmp(userIn, "quit\n")==0 || strcmp(userIn, "\n")==0 || strcmp(userIn, "exit\n")==0) {
                printf("Program Terminated\n");
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                break;
            } */
            /// Send to first Node
            Send(1, userIn, strlen(userIn)+1, comIds);

            printf("Ratings:\n");
            /// Get all Nodes' responses
            for (i = 0; i < numNodes - 1; i++) {
                Receive(buffIn, numNodes - 1, comIds);
                ratings[i] = atoi(buffIn);
                printf("\tNode%d: %d/10\n", i + 1, ratings[i]);
            }
        //}
    } else {
        if (ringSetup(comIds, node, "movieRatingConfig.txt", argv) != 1) {
            printf("Ring setup failed\n");
            return;
        }
        //system("clear");
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        //while (1) {
            Receive(buffIn, node - 1, comIds);        // Receive name first
            memcpy(userIn, buffIn, strlen(buffIn) + 1);

            /*if(strcmp(userIn, "quit\n")==0 || strcmp(userIn, "\n")==0 || strcmp(userIn, "exit\n")==0) {
                printf("Program Terminated\n");
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                break;
            } */
            ratings[node - 1] = getRating(userIn, node);
            for (i = 1; i < node; i++) {
                Receive(buffIn, node - 1, comIds);    // Collect ratings
                ratings[i - 1] = atoi(buffIn);
                printf("Node %d rated this a %d/10\n", i, ratings[i - 1]);
            }
            printf("\nI would give %s a %d/10\n", userIn, ratings[node - 1]);
            if (node < numNodes - 1) {                                   // All nodes before last one
                Send(node + 1, userIn, strlen(userIn) + 1, comIds);       // Send review string
                for (i = 0; i < node; i++) {
                    sprintf(buffOut, "%d", ratings[i]);             // Convert int to string
                    Send(node + 1, buffOut, strlen(buffOut) + 1, comIds); // Send out
                }
            } else {                                                // Last node (send back to start)
                for (i = 0; i < numNodes - 1; i++) {
                    sprintf(buffOut, "%d", ratings[i]);             // Convert int to string
                    Send(0, buffOut, strlen(buffOut) + 1, comIds);      // Send out
                }
            }
        //}
    }
    printf("Program Finished\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    close(comIds[node+1]);
}

int getRating(char * title, int nodeId) {
    int aPart = strlen(title);
    int bPart = title[nodeId % aPart];
    int cPart = ((aPart + bPart) % 10) + 1;

    return cPart;
}