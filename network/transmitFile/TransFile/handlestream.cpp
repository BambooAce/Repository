#include "handlestream.h"
#include <openssl/md5.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
HandleStream::HandleStream()
{
}

HandleStream::~HandleStream()
{

}

std::string HandleStream::calMD5(char *path)
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

int HandleStream::readn(int fd, char *buf, int size)
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

int HandleStream::writen(int fd, char *buf, int size)
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


