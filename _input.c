#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char *argv[])
{
    pid_t old = getpid();
    pid_t new;
    int i;
    for (int i = 0; i < 3; ++i) {
        new = fork();
        if (old == getpid()) {
            printf("Parent Loop: %d \n", i);
        } else {
            const char *args[] = {"./msg.exe", NULL};
            execvp(args[0], args);
        }
    }
    return 1;
}