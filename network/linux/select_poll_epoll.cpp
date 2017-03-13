#include <iostream>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#define MAXCLIENT 1024
using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;
    return 0;
}

int func_select(int listenfd, struct timeval timeout)
{
    fd_set rfds, allfd;
    int fds[MAXCLIENT];
    int i = 0;
    int maxfd = -1;
    for(i; i < MAXCLIENT; ++i)
        fds[i] = -1;
    FD_ZERO(&rfds);
    FD_SET(listenfd, &rfds);
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    maxfd = listenfd;
    int ready = 0, m;
    allfd = rfds;
    char buff[256] = {0};
    while(1)
    {
        int clientfd = -1;
        rfds = allfd;
        ready = select(maxfd + 1, &rfds, NULL, NULL, &timeout);
        if( ready == -1)
        {
            if(errno == EINTR)
                continue;
        }
        if(ready > 0)
        {
            if(FD_ISSET(listenfd, &rfds))
            {
                clientfd = accept(listedfd, (struct sockaddr *) clientaddr, &len);
                if(clientfd)
                    FD_SET(clientfd, &allfd);
                for(m = 0;m < MAXCLIENT; ++m)
                {
                    if(fds[m] < 0)
                    {
                        fds[m] = clientfd;
                        maxfd = maxfd > clientfd  ? maxfd: clientfd;
                        break;
                    }
                }
                if(m == MAXCLIENT)
                    return -1;
                if(--ready <= 0)
                    continue;
            }
            for(int i = 0; i < MAXCLIENT; ++i)
            {
                if (fds[i] > 0)
                {
                    if(FD_ISSET(fds[i], &rfds))
                    {
                        if(read(fds[i], buff, 256) <= 0)
                        {
                            close(fds[i]);
                            FD_CLR(fds[i],&allfd);
                            fds[i] = -1;
                        }
                    }
                }
            }
        }
        else
            continue;
    }
    return 0;
}

int func_poll(int listenfd, struct timeval timeout, int MaxClient)
{
    if(MaxClient < 0)
        MaxClient = 10000;
    struct pollfd clipoll[MaxClient];
    for(int i = 0; i < MaxClient; ++i)
    {
        clipoll[i].fd = -1;
    }
    int maxi = 0;
    int nready = 0;
    int clientfd = 0;
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    clipoll[0].fd = listenfd;
    clipoll[0].events = POLLIN;
    while(1)
    {
        nready = poll(&clipoll, maxi + 1, timeout);
        if(nready == -1)
        {
            if(errno == EINTR)
                continue;
        }
        if(clipoll[0].revents & POLLIN)
        {
            clientfd =  accept(listenfd, (struct sockaddr *)&clientaddr, &len);
            if(clientfd < 0)
                continue;
            for(int i = 1; i < MaxClient; ++i)
            {
                if(clipoll[i].fd < 0)
                {
                    clipoll[i].fd = clientfd;
                    clipoll[i].events = POLLIN;
                    maxi = maxi > i ? maxi: i;
                    break;
                }
            }
            if(--nready <= 0)
                continue;
        }
        for(int i = 0; i < maxi; ++i)
        {
            if(clipoll[i].fd > 0)
            {
                if(clipoll[i].revents & POLLIN)
                {
                    if(read(clipoll[i].fd, buff, 256) <= 0)
                    {
                        close(clipoll[i].fd);
                        clipoll[i].fd = -1;
                    }
                    if(--nready <= 0)
                        continue;
                }
            }
        }
    }
    return 0;
}

int func_epoll(int listenfd)
{
    struct epoll_event a;
    a.events = 100;


}





