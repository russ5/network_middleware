#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

int main(int argc, char *argv[])
{
    int fd = 0,in;
    char buff[1024];

    if(argc<2)
    {
        printf("Missing arguments (IP  Address / Port Num)\n");
        return 0;
    }

//Setup Buffer Array
    bzero(buff,sizeof(buff));

//Create Socket
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd<0)
    {
        perror("Client Error: Socket not created\n");
        return 0;
    }

    struct sockaddr_in server,server1;
    bzero(&server, sizeof(server));
    bzero(&server1, sizeof(server1));

    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr=htonl(INADDR_ANY);

    server1.sin_family = AF_INET;
    server1.sin_port = htons(atoi(argv[1]));
    server1.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(fd,(struct sockaddr*)&server1,sizeof(struct sockaddr_in))==0)
    {
        printf("Bound successfully\n");
    }
    else
        printf("Failed to bind\n");

    socklen_t addr_size=sizeof server;

    in = connect(fd, (struct sockaddr *)&server, sizeof(server));
    if(in<0)
    {
        perror("Client Error: Connection Failed\n");
        return 0;
    }

    while(1)
    {
        printf("\nPlease enter the message: ");
        bzero(buff,1024);
        fgets(buff,1024,stdin);
        if(strncmp(buff,"close",3)==0)
        {
            printf("Connection has been closed\n");
            break;
        }
        printf("\nMessage to server: %s",buff);

        in = write(fd,buff,1024);
        if (in < 0)
        {
            perror("\nClient Error: Writing to Server");
            return 0;
        }
        bzero(buff,1024);
        in = read(fd,buff,1024);
        if (in < 0)
        {
            perror("\nClient Error: Reading from Server");
            return 0;
        }
        if(strncmp(buff,"end",3)==0)
        {
            printf("Server disconnected");
            break;
        }
        printf("\nMessage from server: %s",buff);
    }
    end:
    close(fd);
    return 0;
}