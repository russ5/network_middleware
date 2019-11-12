#include "config.h"

int* get_topology(char* filename){
    FILE *fp;
    char str[MAXCHAR];
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        exit(EXIT_FAILURE);
    }
    static int* topology;
    topology = malloc(sizeof(int) * 2);
    int net_top;
    int line_count = 0;
    while (fgets(str, MAXCHAR, fp) != NULL) {
        if (line_count == 0 && strlen(str) > 3) {
            printf("Please make sure first line of config file is formatted correctly");
            exit(EXIT_FAILURE);
        } else if (line_count == 0 && strlen(str) <= 3) {
            topology[0] = atoi(str);
        }
        line_count++;
    }
    topology[1] = --line_count;
    fclose(fp);
    return topology;
}

struct machine* get_machines(char* filename){
    FILE *fp;
    char str[MAXCHAR];
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        exit(EXIT_FAILURE);
    }

    static struct machine* machines;
    machines = malloc((sizeof(int) + sizeof(char) * 30) * 15);
    int line_count = 0;
    char *pt;
    while (fgets(str, MAXCHAR, fp) != NULL) {
        if (line_count > 0) {
            pt = strtok(str, ",");
            int is_it_ip = 1;
            while(pt != NULL){
                if(is_it_ip == 1){
                    strcpy(machines[line_count - 1].ip_address, pt);
                    is_it_ip = 0;
                }
                else if(is_it_ip == 0){
                    machines[line_count - 1].port = atoi(pt);
                }
                pt = strtok(NULL, ",");
            }
        }
        line_count++;
    }
    fclose(fp);
    return machines;
}
