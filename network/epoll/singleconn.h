#ifndef SINGLECONN_H
#define SINGLECONN_H
#include <netinet/in.h>
#include "mem.h"
class SingleConn
{
public:
    SingleConn(struct sockaddr_in client, int fd);
    ~SingleConn();
    SingleConn(const SingleConn &sc);
    SingleConn & operator=(const SingleConn &sc);
    bool operator==(const SingleConn &sgc) const;
    bool operator <(const SingleConn &clm) const
    {
        return clifd < clm.clifd;
    }
    void recvHeader(int datasize);
    bool hadRecvHeader();
    int fillIntoData(char *buff, int len);
    void pushIntoMem(MEM *mem);
public:
    struct sockaddr_in cliaddr;
    int clifd;
    bool header;
    int size;
    char *data;
    int haveRecved;
};

#endif // SINGLECONN_H
