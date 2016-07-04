#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(7777);
    inet_pton(AF_INET, argv[1],(void *)&serveraddr.sin_addr);
    char buff[2048*2048];
    memset(buff, 0, 2048*2048);
    connect(clientfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr));
    while(!feof(stdin))
    {
       size_t getsize = fread(buff,1,2048*2048,stdin);
       if(getsize){
            write(clientfd, buff, getsize);
       }

    }

    close(clientfd);
}

