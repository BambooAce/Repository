#include "fileserver.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>

#define GET "GET:"
#define PUT "PUT:"

FileServer::FileServer():serverfd(-1), port(0)
{

}

FileServer::~FileServer()
{
    if(serverfd)
        close(serverfd);
}

void FileServer::init(int iport)
{
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd < 0)
        exit(-1);
    port = iport;
    struct sockaddr_in seraddr;
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(port);
    seraddr.sin_addr.s_addr = INADDR_ANY;
    if(bind(serverfd, (const struct sockaddr *)&seraddr, sizeof(seraddr)) == -1)
    {
        if(errno == EADDRINUSE)
            fprintf(stderr, "bind failed: ADDRINUSE\n");
        exit(-1);
    }
    int reuse = 1;
    if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEPORT, (void *)&reuse, sizeof(reuse)) == -1)
    {
        fprintf(stderr, "setsockopt: reuseport error\n");
        exit(-1);
    }
    int size = 1048576;
    if(setsockopt(serverfd, SOL_SOCKET, SO_RCVBUF, (void*)&size, sizeof(size)) == -1)
    {
        fprintf(stderr, "setsockopt: so_rcvbuf error\n");
        exit(-1);
    }
    if(setsockopt(serverfd, SOL_SOCKET, SO_SNDBUF, (void*)&size, sizeof(size)) == -1)
    {
        fprintf(stderr, "setsockopt: so_sndbuf error\n");
        exit(-1);
    }
    listen(serverfd, 10);
}

int main()
{
    FileServer fs;
    fs.init(6666);
    fs.thread_run();
}
