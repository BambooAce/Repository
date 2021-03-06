#include "fileclient.h"
#include "handlestream.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
enum {
    PUT,
    GET
};

FileClient::FileClient(std::string url, int iport):urladd(url), port(iport)
{
    clifd = socket(AF_INET, SOCK_STREAM, 0);
    struct linger lg;
    lg.l_onoff = 1;
    lg.l_linger = 5;
    socklen_t len = sizeof(lg);
    setsockopt(clifd, SOL_SOCKET, SO_LINGER, &lg, len);
    if(clifd <= 0)
    {
        fprintf(stderr, "socket function fail\n");
        _exit(1);
    }
}

FileClient::~FileClient()
{
    if(clifd > 0)
        shutdown(clifd, SHUT_WR);
}

std::string FileClient::setHeader(int mode, std::string &filename, int filesize, std::string md5)
{
    if(mode == PUT)
    {
        if(!filename.empty())
            return msg.createPutMsg(filesize, md5, filename);
    }else if(mode == GET)
    {
        if(!filename.empty())
            return msg.createGetMsg(filename);
    }
    return "";
}

bool FileClient::connServer()
{
    if(urladd.empty())
        return false;
    std::string ipaddr = getIPaddr(urladd);
	std::cout << ipaddr << std::endl;
    if(ipaddr.empty())
    {
        fprintf(stderr, "Get Server IP failed\n");
        return false;
    }
    struct sockaddr_in seraddr;
    seraddr.sin_port = htons(port);
    seraddr.sin_family = AF_INET;
    inet_aton(ipaddr.c_str(), &seraddr.sin_addr);
    struct timeval tval;
    tval.tv_sec = 10;
    tval.tv_usec = 100;
    int fl = fcntl(clifd, F_GETFL);
    if(fl == -1)
    {
        fprintf(stderr, "fcntl fail\n");
        return false;
    }
    fl = fcntl(clifd, F_SETFL, fl | O_NONBLOCK);
    if(fl == -1)
    {
        fprintf(stderr, "fcntl fail\n");
        return false;
    }
    fd_set wfd;
    FD_ZERO(&wfd);
    FD_SET(clifd, &wfd);
    do{
        int res = connect(clifd, (const struct sockaddr*)&seraddr, sizeof(seraddr));
        if(res == 0)
        {
            return true;
        }
        if(errno == EINTR)
        {
            continue;
        }
        if(errno == EINPROGRESS){

        sign:
            int ready = select(clifd + 1, NULL, &wfd, NULL, &tval);
            if(ready == -1)
            {
                if(errno == EINTR)
                {
                    goto sign;
                }
            }
            if(ready == 0)
            {
                fprintf(stderr, "Connect server timeout\n");
				return false;
               // _exit(1);
            }
            if(FD_ISSET(clifd, &wfd))
            {
                int err;
                socklen_t len = sizeof(err);
                if(getsockopt(clifd, SOL_SOCKET, SO_ERROR, (void *)&err, &len) == -1)
                {
                    fprintf(stderr, "getsockopt failed\n");
                    _exit(1);
                }
                if(err == 0)
                {
                    return true;
                }else if(err == ECONNREFUSED)
				{
					fprintf(stderr, "Server no listenning\n");
					return false;
				}else if(err == ENETUNREACH)
				{
					fprintf(stderr, "Network can`t reach\n");
					return false;
				}
                else
                    return false;
            }
        }
    }while(0);
    return false;
}

void FileClient::sendHeader(std::string header)
{
    char buff[256] = {0};
    const char * str = header.c_str();
    memcpy(buff, str, strlen(str));
    if(clifd)
    {
        writen(clifd, buff, strlen(buff));
    }
}

void FileClient::sendFile(FILE *fp)
{
    if(fp)
    {
        while(!feof(fp)){
            char buff[1024*2] = {0};
            int size = fread(buff, 1024, 2, fp);
            writen(clifd, buff, size);
        }
        fclose(fp);
    }
}

void FileClient::recvFile(FILE *fp)
{
    if(fp)
    {
        while(!feof(fp)){
            char buff[1024*2] = {0};
            int size = readn(clifd, buff, 2048);
            fwrite(buff, size, 1, fp);
        }
        fclose(fp);
    }
}
