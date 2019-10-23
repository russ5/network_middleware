#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    pid_t c_pid;
    c_pid = fork();

    if(c_pid == 0) {
        printf("This is the child");
    }
}