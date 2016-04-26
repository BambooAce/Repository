#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#define ERROR(CALL) { \
    perror(#CALL "error"); \
    fprintf(stderr, "%s\n", strerror(errno)); \
    exit(-1); \
    }

int main(int argc, char* args[])
{
    if(argc != 2)
    {
        fprintf(stderr, "USE : ./client ServerAddr\n");
        exit(-1);
    }
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > clientfd)
        ERROR(socket);
    struct sockaddr_in serversock_addr;
    serversock_addr.sin_family = AF_INET;
    serversock_addr.sin_port = htons(8000);
    inet_aton(args[1], &serversock_addr.sin_addr);
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fileno(stdin), &rfds);
    FD_SET(clientfd, &rfds);
    int maxfd = clientfd > fileno(stdin)? clientfd: fileno(stdin);
    int conn = connect(clientfd, (const sockaddr *)&serversock_addr, sizeof(serversock_addr));
    if (0 > conn)
        ERROR(connect);
    while(1)
    {
        int ready = select(maxfd + 1, &rfds, NULL, NULL, NULL);
        //
        //
        // *************************************************
        // After select function all fd_set will set NULL !!!
        // so we must restore its fd_set. Fristly, we should
        // make a copy to fd_set.
        // *************************************************
        //
        if (0 > ready)
        {
            ERROR(select);
        }
        else if(0 == ready)
        {
            FD_ZERO(&rfds);
            FD_SET(fileno(stdin), &rfds);
            FD_SET(clientfd, &rfds);
            continue;
        }
        if (ready > 0)
        {
            if (FD_ISSET(fileno(stdin), &rfds))
            {
                memset(buff, 0, sizeof(buff));
                size_t readed = 0;
                if ((readed = read(0, buff, sizeof(buff))) > 0)
                {
                    write(clientfd, buff, readed);
                }
            }
            if (FD_ISSET(clientfd, &rfds))
            {
                memset(buff, 0, sizeof(buff));
                size_t readed = 0;
                if ((readed = read(clientfd, buff, sizeof(buff))) <= 0)
                {
                    break;
                }
                else
                {
                    write(1, buff, readed);
                }
            }
        }
        FD_ZERO(&rfds);
        FD_SET(fileno(stdin), &rfds);
        FD_SET(clientfd, &rfds);
    }
    close(clientfd);
    return 0;
}
















