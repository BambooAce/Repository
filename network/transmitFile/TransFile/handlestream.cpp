#include "handlestream.h"
#include <openssl/md5.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <iostream>

std::string calMD5(char *path)
{
    FILE *fp = fopen(path, "rb");
    if(!fp)
    {
        fprintf(stderr, "Open file fail.\n");
        return "";
    }
    int i = 0;
    MD5_CTX md5;
    char md5str[33] = {0};
    unsigned char md[16] = {0};
    char amd[3] = {0};
    char buff[1024*1024] = {0};
    int ready = 0;
    MD5_Init(&md5);
    printf("hello\n");
    while(!feof(fp))
    {
        ready = fread(buff, 1024, 1024, fp);
        if(errno == EINTR)
            continue;
        MD5_Update(&md5, buff, ready);
    }
    fclose(fp);
    MD5_Final(md, &md5);
    for(; i < 16; ++i)
    {
        sprintf(amd, "%02X", md[i]);
        strcat(md5str, amd);
    }
    return std::string(md5str);
}

int readn(int fd, char *buf, int size)
{
    int ready, left, n = 0;
    left = size;
    char * pbuff = buf;
    while(left)
    {
        ready = read(fd, pbuff, left);
        if(errno == EINTR && ready == -1)
        {
            continue;
        }
        if(ready == 0)
            break;
        left -= ready;
        pbuff += ready;
        n += ready;
    }
    return n;
}

int writen(int fd, char *buf, int size)
{
    int ready, left, n = 0;
    left = size;
    char * pbuff = buf;
    while(left)
    {
        ready = write(fd, pbuff, left);
        if(errno == EINTR && ready == -1)
        {
            continue;
        }
        if(ready == 0)
            break;
        left -= ready;
        pbuff += ready;
        n += ready;
    }
    return n;
}

std::string getIPaddr(std::string url)
{
    fprintf(stderr, "%s\n", url.c_str());
    if(url.empty())
        return "";
    int status = 0;
    struct addrinfo hints, *res, *p;
    const char * urladdr = url.c_str();
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if((status = getaddrinfo(urladdr, NULL, &hints, &res)) != 0)
    {
        fprintf(stderr, "%s\n", gai_strerror(status));
        return "";
    }
    void *addr;
    char buf[128] = {0};
    socklen_t len;
    for(p = res; p != NULL; p = p->ai_next)
    {
        if(p->ai_family == AF_INET)
        {
            struct sockaddr_in * addripv4 = (struct sockaddr_in *)(p->ai_addr);
            addr = &(addripv4->sin_addr);
            len = INET_ADDRSTRLEN;
        }
        else if(p->ai_family == AF_INET6)
        {
            struct sockaddr_in6 * addripv6 = (struct sockaddr_in6 *)(p->ai_addr);
            addr = &(addripv6->sin6_addr);
            len = INET6_ADDRSTRLEN;
        }
        inet_ntop(p->ai_family, addr, buf, len);
        break;
    }
    freeaddrinfo(res);
    return std::string(buf);
}

int getSize(char *path)
{
    struct stat filestat;
    int status = stat(path, &filestat);
    if(status == 0)
    {
        return filestat.st_size;
    }
    return -1;
}

std::string parseFilename(char *path)
{
    std::string temp(path);
    size_t pos = temp.find_last_of('/');
    if(pos == std::string::npos)
        return temp;
    return temp.substr(pos+1, temp.length());
}
