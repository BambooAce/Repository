#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#define PORT 6666
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "+++ IPaddress\n");
        return -1;
    }
    int clientfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(clientfd < 0)
        return -1;
    struct sockaddr_in serveraddr;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_family = AF_INET;
    char *buff = "hello world";
    size_t n = strlen(buff);
    char rebuff[256] = {0};
    if(inet_pton(AF_INET, argv[1], (void *)&serveraddr.sin_addr) == 0)
    {
        printf("IP address error\n");
        return -1;
    }
    socklen_t len = sizeof(serveraddr);
    while(1)
    {
        sendto(clientfd, buff, n, 0, (const struct sockaddr *)&serveraddr, len);
        printf("hello\n");
        recvfrom(clientfd, rebuff, 256, 0, NULL, NULL);
    }
    return 0;
}
