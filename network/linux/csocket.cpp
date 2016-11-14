#include <string>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <fcntl.h>
#include "csocket.h"

inline void HandleError(std::string func)
{
    char errmsg[256];
    bzero(errmsg, 256);
    snprintf(errmsg, 256, "%s: %s\n", func.c_str(), strerror(errno));
    write(2,errmsg,strlen(errmsg));
    exit(1);
}

Csocket::Csocket():sockfd(-1)
{
}

Csocket::~Csocket()
{
    if(sockfd > 0)
        close(sockfd);
}

bool Csocket::Connect(char * addr, unsigned short port, int timeout)
{
    struct sockaddr_in seraddr;
    struct timeval tval;
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(port);
    int res = inet_pton(AF_INET, addr, &seraddr.sin_addr);
    if((res == 1) && (timeout > 0)){
        SetNONBlock();
        tval.tv_sec = timeout;
        tval.tv_usec = 100;
        fd_set wfd, rfd;
        FD_ZERO(&wfd);
        FD_ZERO(&rfd);
        FD_SET(sockfd, &wfd);
        FD_SET(sockfd, &rfd);
        int resconn = connect(sockfd, (const sockaddr *)&seraddr, sizeof(seraddr));
        if(resconn == 0)
        {
            write(1, "Connection success.\n",50);
            return true;
        }
        if (resconn == -1)
        {
            if(errno == EINPROGRESS)
            {
                int nready = select(sockfd+1, &rfd, &wfd, NULL, &tval);
                if(nready == -1)
                {
                    close(sockfd);
                    HandleError("Select");
                }else if(nready == 0)
                {
                    write(2, "Connect Timeout!\n", 50);
                    close(sockfd);
                    exit(0);
                }else
                {
                    int err;
                    socklen_t len = sizeof(err);
                    if(FD_ISSET(sockfd, &wfd) || FD_ISSET(sockfd, &rfd))
                    {
                        if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (void *)&err, &len) == -1)
                        {
                            close(sockfd);
                            HandleError("Getsockopt");
                        }
                        if(err == 0)
                        {
                            write(1, "Connect success.\n", 50);
                            return true;
                        }else{
                            close(sockfd);
                            errno = err;
                            HandleError("Connect");
                        }
                    }
                }
            }
        }
    }
    else if((res == 1) && (timeout == 0))
    {
        int re = connect(sockfd, (const sockaddr *)&seraddr, sizeof(seraddr));
        if(re == 0)
        {
            write(1, "Connect success.\n", 50);
            return true;
        }else{
            close(sockfd);
            HandleError("Connect");
        }
    }
    else if( res!=1 )
    {
        close(sockfd);
        HandleError("Inet_pton");
    }
    return false;
}

bool Csocket::Listen()
{
    if(listen(sockfd, 4) != 0)
    {
        close(sockfd);
        HandleError("Listen");
    }
    return true;
}

int Csocket::Accept()
{
    int clientfd = accept(sockfd, NULL, NULL);
    if(clientfd < 0)
    {
        close(sockfd);
        HandleError("Accept");
    }
    return clientfd;
}

bool Csocket::Bind(struct sockaddr_in &seraddr)
{
    if(bind(sockfd, (const sockaddr *)&seraddr, sizeof(seraddr)) != 0)
    {
        close(sockfd);
        HandleError("Bind");
    }
    return true;
}

int Csocket::Socket(bool TCP)
{
    int SOCK = TCP ? SOCK_STREAM:SOCK_DGRAM;
    sockfd = socket(AF_INET,SOCK, 0);
    if (-1 == sockfd)
    {
        HandleError("Socket");
    }
    return sockfd;
}

bool Csocket::SetNONBlock()
{
    if(sockfd > 0)
    {
        int flags = fcntl(sockfd, F_GETFL, 0);
        int res = fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
        if(-1 == res)
        {
            close(sockfd);
            HandleError("Fcntl");
        }
        return true;
    }
    return false;
}

bool Csocket::SetSock()
{
    return true;
}

int Csocket::GetFd()
{
    return sockfd;
}


void ShowClient(int clientfd)
{
    if(clientfd)
    {
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        getpeername(clientfd, (struct sockaddr*)&clientaddr, &len);
        char clientmsg[256];
        bzero(clientmsg, 256);
        char *addr = inet_ntoa(clientaddr.sin_addr);
        snprintf(clientmsg, 256, "Client: %s\n", addr);
        write(1, clientmsg, 256);
    }
}


bool CreateUDPServer(Csocket server, unsigned short port)
{

}


bool CreateTCPServer(Csocket server, unsigned short port)
{
    server.Socket();
    struct sockaddr_in seraddr;
    seraddr.sin_addr.in_addr = htonl();
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(port);
    server.Bind(seraddr);
    server.Listen();
}


bool CreateUDPClient(Csocket client, char * addr, unsigned short port)
{
    client.Socket(false);
}


bool CreateTCPClient(Csocket client, char * addr, unsigned short port, int timeout)
{
    client.Socket();
    if(client.Connect(addr, port, timeout))
        return true;
    return false;
}
