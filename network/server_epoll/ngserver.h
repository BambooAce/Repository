#ifndef INFOSERVER_H
#define INFOSERVER_H
#include "iniparser.h"
#include "mem.h"
#include "singleconn.h"
#include <list>
#include <stdio.h>
typedef struct sc{
    SingleConn sgc;
    time_t t_time;
    sc(struct sockaddr_in saddr, int fd, time_t t):sgc(saddr, fd), t_time(t)
    {
    }
    ~sc()
    {
        if(sgc.data)
        {
            free(sgc.data);
            sgc.data = NULL;
            fprintf(stderr, "data delete\n");
        }
    }
}SC;

class InfoServer
{
public:
    InfoServer(SINFO &sinfo);
    ~InfoServer();
    int bind_socket();
    int accept_epoll(int listen_sock, MEM *mem);
private:
    int epollfd;
    SINFO serInfo;
    std::list<SC> AllConns;
    std::list<SC>::iterator it;
    void record_conn(struct sockaddr_in cliaddr, int clifd);
    void delete_normal_conn(int clifd);
    void delete_unnormal_conn();
    void delete_special_conn(int clifd);
};

#endif // INFOSERVER_H
