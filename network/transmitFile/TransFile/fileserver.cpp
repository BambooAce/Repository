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
#include <pthread.h>
#include <string.h>
#include <openssl/md5.h>
#include <map>
#include <string>
#include <iostream>
#include <iterator>
#include <mysql/mysql.h>
#define GET "GET:"
#define PUT "PUT:"

FileServer::FileServer():serverfd(-1), port(0)
{
    getmd5("./md5file");
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

void FileServer::getmd5(std::string md5file)
{
    if( 0 != access(md5file.c_str(), F_OK))
    {
        system("touch md5file");
        return;
    }
    char buf[256] = {0};
    FILE *fp = fopen(md5file.c_str(), "r");
    if(fp)
    {
        while(fgets(buf, 256, fp))
        {
            size_t len = strlen(buf);
            if(buf[len-1] == '\n')
                buf[len-1] = '\0';
            if(len == 1)
                continue;
            char * p = strchr(buf, ' ');
            char *filename = p + 1;
            char md5[40] = {0};
            strncpy(md5, buf, p - buf);
            size_t len2 = strlen(md5);
            md5[len2] = '\0';
            memset(buf, 0, 256);
            std::string mdkey(md5);
            std::string namevalue(filename);
            mdfilemap.insert(std::pair<std::string, std::string>(mdkey, namevalue));
        }
    }
}

static void *call_back(void *arg)
{
    int * client = (int *)arg;
    int clientfd = *client;
    char buf[256] = {0};
    char mem[1024*1024] = {0};
    int size = read(clientfd, buf, 256);
    if(size > 4){
        buf[size] = '\0';
        if(strncmp(buf, PUT, 4) == 0)
        {
            char * filename =  &buf[4];
            int fd = open(filename,O_CREAT|O_EXCL);
            while(1)
            {
                int recvsize = read(clientfd, mem, 1024*1024);
                if(recvsize > 0)
                {
                    write(fd, mem, recvsize);
                }
                else{
                    close(fd);
                    close(clientfd);
                    break;
                }
            }
        }
        if(strncmp(buf, GET, 4) == 0)
        {
            char * filename =  &buf[4];
            if(0 != access(filename, F_OK))
            {
                std::string erro = "NOTHISFILE";
                if(write(clientfd, erro.c_str(), erro.length()) > 0)
                {
                    close(clientfd);
                }
            }
            int fd = open(filename, O_RDONLY);
            while(1)
            {
                int recvsize = read(fd, mem, 1024*1024);
                if(recvsize > 0)
                {
                    write(clientfd, mem, recvsize);
                }
                else{
                    close(fd);
                    close(clientfd);
                    break;
                }
            }
        }
    }
}

void FileServer::thread_run()
{
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    while(1){
        fd_set rfd;
        FD_ZERO(&rfd);
        FD_SET(serverfd, &rfd);
        int ready = select(serverfd, &rfd, NULL, NULL, NULL);
        if(ready)
        {
            if(FD_ISSET(serverfd, &rfd))
            {
                int clientfd = accept(serverfd, (struct sockaddr*)&clientaddr, &len);
                if(clientfd){
                    pthread_t child;
                    pthread_create(&child, 0, call_back, (void *)&clientfd);
                }
            }
        }else
            continue;
    }
}


int main()
{
    FileServer fs;
    fs.init(6666);
    fs.thread_run();
}
