#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
int main()
{
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(7777);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(serverfd, (const struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(serverfd, 20);
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(clientaddr);
    char buff[2048*2048];
    memset(buff, 0, 2048*2048);
    while(1)
    {
        int clientfd = accept(serverfd,(struct sockaddr*)&clientaddr, &addrlen);
        do
        {
           size_t have = read(clientfd, buff, 2048*2048);
           if(have == 0)
           {
               break;
           }
           printf("%s\n", buff);

        }while(1);
        sleep(1);
        close(clientfd);
    }
    return 0;
}
