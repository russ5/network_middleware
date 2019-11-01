//
// Created by TY on 10/31/2019.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
 First line of text file is an integer that corresponds to a network topology.
 0 = point to point (only two computers)
 1 = bus
 2 = ring
 3 = star
 4 = fully connected

 Any subsequent lines will hold the IP addresses & ports of the machines in the network. (Comma separated)
*/

int main() {
    FILE *file;
    // Dynamically reads line by line of file
    size_t maxl = 256;
    char *line = malloc(maxl * sizeof(char));
    if(!line){
        printf("Memory not allocated!!\n");
        return -2;
    }
    file = fopen(".\\config.txt", "r");

    int line_num = 0;
    while (fgets(line, maxl, file)) {
        while(line[strlen(line) - 1] != '\n' && line[strlen(line) - 1] != '\r'){
            char *tmp = realloc (line, 2 * maxl * sizeof(char));
            fseek(file,0,SEEK_SET);
            if (tmp) {
                line = tmp;
                maxl *= 2;
                fgets(line, maxl, file);
            }
            else{
                printf("Not enough memory for this line!!\n");
                return -3;
            }
        }

        // Error check format of file
        if(line_num == 0 && strlen(line) != 3){
            printf("First line of txt is not one character long");
            printf("%d", strlen(line));
        }
        else if(line_num > 0){
            // Split up lines by comma delimiter
            char delim[] = ",";
            char *ptr = strtok(line,delim);
            while(ptr != NULL){
                printf("'%s'\n", ptr);
                ptr = strtok(NULL, delim);
            }
        }
        line_num++;

    }
}