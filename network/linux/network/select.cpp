#include "select.h"
#include "../csocket.h"
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#define MAXCLIENT 1024
Select::Select()
{
}

void Select::Run(Csocket &server)
{
    int fd = server.GetFd();
    fd_set rset, allset;
    FD_ZERO(&rset);
    FD_ZERO(&allset);
    FD_SET(fd, &allset);
    int maxfd, maxi;
    int clientfd[MAXCLIENT];
    for(int i = 0;i < MAXCLIENT; ++i)
    {
        clientfd = -1;
    }
    maxfd = fd;
    maxi = -1;
    int nready;
    int m;
    for(;;)
    {
        struct timeval tval;
        tval.tv_sec = 10;
        tval.tv_usec = 100;
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, &tval);
        if(nready == -1)
        {
            if(errno == ENTER)
                continue;
        }else if(nready == 0)
        {
            continue;
        }
        if(FD_ISSET(fd,&rset))
        {
            int client = server.Accept();
            for(m = 0;m < MAXCLIENT; ++m)
            {
                if(clientfd[m] < 0)
                {
                    clientfd[m] = client;
                    break;
                }
            }
            if(m == MAXCLIENT)
                write(2, "Too much client", 30);
            maxfd = client > maxfd? client:maxfd;
        }

        if(--nready <= 0)
            continue;
        for(int i = 0; i < MAXCLIENT; ++i)
        {

        }
    }
}
