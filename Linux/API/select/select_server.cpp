#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <stdlib.h>

#define ERROR(CALL) { \
    perror(#CALL "error"); \
    fprintf(stderr, "%s\n", strerror(errno)); \
    exit(-1); \
    }
#define MAXLOG 20
int main(void)
{
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd < 0)
        ERROR(socket);
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8000);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(serverfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
        ERROR(bind);
    if (-1 == listen(serverfd, MAXLOG))
        ERROR(listen);
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    while(1)
    {
        int clientfd = 0;
        if ((clientfd = accept(serverfd, (struct sockaddr *)&clientaddr, &len)) > 0)
        {
            int pid = fork();
            if (pid > 0)
            {
                close(clientfd);

            }
            else if(pid == 0)
            {
                char buf[512];
                printf("client is %s\n",inet_ntoa(clientaddr.sin_addr));
                read(0,buf,512);
            }

        }
    }
    close(serverfd);
    return 0;
}

