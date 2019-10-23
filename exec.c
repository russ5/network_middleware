#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    char * ls_args[] = {"./print", "-1", NULL};
    execv(ls_args[0], ls_args);
    perror("execv");
    return 2;
}