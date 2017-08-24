#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "netmode.h"
#include "connclient.h"

#ifdef __cplusplus
#include <list>
std::list<CONNCLIENT> connlist;
#else
#include "connlist.h"
CONNCLIENT * connlist = NULL;
#endif

void select_work(int lisfd)
{
    sockaddr_in a;
}

void poll_work(int lisfd)
{

}

void epoll_work(int lisfd)
{

}

void acceptHandler()
{

}

void readHandler()
{

}

void writeHandler()
{

}

void IO_multiplexing(WORKPROCESS work_process, void *arg)
{

}
