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

int main()
{
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( 0 > clientfd )
        ERROR(SOCKET);
    struct sockaddr_in serveraddr;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8000);
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    if ( -1 != connect(clientfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr)))
    {
        while(1)
        {
            fgets(buf, sizeof(buf), stdin);
            if (strlen(buf)>0)
            {
                write(clientfd, buf, strlen(buf));
                memset(buf, 0, sizeof(buf));
                read(clientfd, buf, sizeof(buf));
                write(STDOUT_FILENO, buf, strlen(buf));
                memset(buf, 0, sizeof(buf));
            }
        }
        close(clientfd);

    }
    return 0;
}
