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

#define FormatError(error) do{ \
        fprintf(stderr, error); \
        return 1; \
    }while(0)


enum {
    PUT,
    GET
};

void * connectServer(void *arg)
{

}

void * calFileMD5(void *arg)
{

}

FileClient::FileClient(std::string url, int iport):urladd(url), port(iport)
{
    clifd = socket(AF_INET, SOCK_STREAM, 0);
    if(clifd <= 0)
    {
        fprintf(stderr, "socket function fail\n");
        _exit(1);
    }
}

FileClient::~FileClient()
{
    if(clifd > 0)
        close(clifd);
}

std::string FileClient::setHeader(int mode, std::string filename, int filesize, std::string md5)
{
    if(mode == PUT)
    {
        if(!filename.empty())
            return msg.createPutMsg(filesize, md5, filename);
    }else if(mode == GET)
    {
        if(!filename.empty())
            return msg.createGetMsg(filename);
    }else
        return "";
}

bool FileClient::connServer()
{
    if(urladd.empty())
        return false;
    std::string ipaddr = getIPaddr(urladd);
    struct sockaddr_in seraddr;
    seraddr.sin_port = htons(7682);
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
        if(errno = EINTR)
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
                _exit(1);
            }
            if(FD_ISSET(clifd, &rfd))
            {
                int err;
                len = sizeof(err);
                if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (void *)&err, &len) == -1)
                {
                    fprintf(stderr, "getsockopt failed\n");
                    _exit(1);
                }
                if(err == 0)
                {
                    return true;
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
    if(clifd)
    {
        writen(clifd, buff, size)
    }
}


int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        FormatError("Format : fileclient PUT/GET filename serverIP\n");
    }
    int mode = 0;
    if(strcmp(argv[1], "PUT") == 0 || strcmp(argv[1], "put") == 0)
        mode = 0;
    else if(strcmp(argv[1], "GET") == 0 || strcmp(argv[1], "get") == 0)
        mode = 1;
    else{
        FormatError("Format : fileclient PUT/GET filename serverIP\n");
    }
    if((access(argv[2], F_OK) == 0) && mode)
    {
        FormatError("File exist\n");
    }else if((access(argv[2], F_OK) == -1) && !mode)
    {
        FormatError("File not exist\n");
    }
//    create thread cal file md5
//    main thread client connect server.
    FileClient client;
    client.connect();
    client.setHeader(mode, )
    return 0;
}
