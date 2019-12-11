#include "mw_internal.h"

struct Config readConfig(char * configPath) {

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

int reachMiddleware() { // char * args[] here, or build it?

}

int connect(char * ip, int sockId) {

}

int listenAccept(char * ip, int sockId) {

}