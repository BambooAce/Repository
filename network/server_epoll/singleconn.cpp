#include "singleconn.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
SingleConn::SingleConn(struct sockaddr_in client, int fd):header(false), size(0),data(NULL), haveRecved(0)
{
    bzero(&cliaddr, sizeof(cliaddr));
    memcpy(&cliaddr, &client, sizeof(struct sockaddr_in));
    clifd = fd;
}

SingleConn::~SingleConn()
{
    if(data){
        fprintf(stderr, "singconn data be deleted");
        free(data);
        data = NULL;
    }
}

SingleConn::SingleConn(const SingleConn &sc)
{
   memcpy(&cliaddr, &(sc.cliaddr), sizeof(cliaddr));
   clifd = sc.clifd;
   haveRecved = sc.haveRecved;
   header = sc.header;
   size = sc.size;
   if(sc.data)
   {
       data = (char *)malloc(sc.size);
       bzero(data, sc.size);
       memcpy(data, sc.data, sc.haveRecved);
   }else{
       data = NULL;
   }
}

SingleConn &SingleConn::operator=(const SingleConn &sc)
{
    memcpy(&cliaddr, &(sc.cliaddr), sizeof(cliaddr));
    clifd = sc.clifd;
    haveRecved = sc.haveRecved;
    header = sc.header;
    size = sc.size;
    if(sc.data)
    {
        data = (char *)malloc(sc.size);
        bzero(data, sc.size);
        memcpy(data, sc.data, sc.haveRecved);
    }else{
        data = NULL;
    }
    return *this;
}

/**
 * @brief SingleConn::operator ==
 * @param sgc
 * @return
 * check same connection
 */
bool SingleConn::operator==(const SingleConn &sgc) const
{
    return (clifd == sgc.clifd ) && (0 == memcmp(&cliaddr, &sgc.cliaddr, sizeof(struct sockaddr_in)));
}

/**
 * @brief SingleConn::recvHeader
 * @param datasize
 * have recv header
 */
void SingleConn::recvHeader(int datasize)
{
    header = true;
    size = datasize;
}

bool SingleConn::hadRecvHeader()
{
    return header;
}

/**
 * @brief SingleConn::fillIntoData
 * @param buff
 * @param len
 * @return if data have recved complete, return 1
 * else return 0
 * will fill buff into mem until data complete
 */
int SingleConn::fillIntoData(char *buff, int len)
{
    fprintf(stderr, "size %d, haveRecved %d, len %d", size, haveRecved, len);
    if(size < (haveRecved + len)){
        memcpy(data+haveRecved, buff, size - haveRecved);
        haveRecved = size;
        return 1;
    }
    if(data == NULL)
    {
        data = (char *)malloc(size);
    }
    memcpy(data + haveRecved, buff, len);
    haveRecved += len;
    if(haveRecved == size)
        return 1;
    return 0;

}

/**
 * @brief SingleConn::pushIntoMem
 * @param mem
 * when data complete, put data into mem
 */
void SingleConn::pushIntoMem(MEM *mem)
{
    if(mem){
        mem->pushData(data, haveRecved);
        if(data){
            free(data);
            data = NULL;
        }
    }
}
