#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../mw_internal.h"
#include "../mw_public.h"

void main(int argc, char const *argv[]) {
    int i;
    int id;
    char inst[1024];

    printf("Test worked\n");
    for(i=0; i<10; i++) {
        printf("%d ", i);
    }

    /*
    id = listenAccept(59001);
    read(id, inst, 1024);
    printf("%s\n", inst);
    close(id);
    */
}