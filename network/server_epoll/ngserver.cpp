/**
 * EPOLL server accept client data. And delete unnormal connections.
 *
*/
#include "ngserver.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <map>
#include <utility>
#include <errno.h>
#include <fcntl.h>
#include <algorithm>
#include "streamoper.h"
#include "log.h"

//Max client count.
#define EVENTSMAX 100000
//Listen and accept queue length
#define BACKLOG 100
//Timeout time, if client above it, server will send RST to client.
#define EPOLLTIMEOUT 8000

const time_t ComTime = 1500000000;

/**
 * To handle system call fail.
 */
#define HandleError(msg) do{ \
    fprintf(stderr, msg);\
    _exit(1);\
    }while(0)

#define FINDCLIENT(sclient) do{ \
    for(it = AllConns.begin(); it != AllConns.end(); ++it)  \
{   \
    if(it->sgc == (sclient)){  \
    break;  \
    }   \
    }   \
    }while(0)

/**
 * @brief InfoServer::InfoServer
 * @param sinfo: Server infomation read from configuration file.
 */
InfoServer::InfoServer(SINFO &sinfo):serInfo(sinfo), epollfd(-1)
{

}

InfoServer::~InfoServer()
{
    if(epollfd)
        close(epollfd);
}

/**
 * @brief send_RST
 * @param clifd : client file desc.
 * set fd to send RST
 */
static void send_RST(int clifd)
{
    struct linger lger;
    lger.l_onoff = 1;
    lger.l_linger = 0;
    if(clifd){
        setsockopt(clifd, SOL_SOCKET, SO_LINGER, &lger, sizeof(lger));
        close(clifd);
    }
}

/**
 * @brief set_noblock
 * @param clifd
 * set client fd is no block
 */
static void set_noblock(int clifd)
{
    int flags = fcntl(clifd, F_GETFL);
    if(flags != -1)
    {
        fcntl(clifd, F_SETFL, flags | O_NONBLOCK);
    }
}


/**
 * @brief InfoServer::bind_socket
 * @return listen fd.
 * Init server
 */
int InfoServer::bind_socket()
{
    epollfd = epoll_create(EVENTSMAX);
    if(epollfd == -1)
    {
        HandleError("epoll_create error\n");
    }
    int serfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serfd <= 0)
    {
        HandleError("socket failed\n");
    }
    struct sockaddr_in seraddr;
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(serInfo.port);
    if(serInfo.ipAddr == "ANY")
        seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        inet_aton(serInfo.ipAddr.c_str(), &seraddr.sin_addr);
    socklen_t len = sizeof(seraddr);
    if( -1 == bind(serfd, (const struct sockaddr *)&seraddr, len))
    {
        HandleError("bind failed\n");
    }
    int reuse = 1;
    socklen_t l = sizeof(reuse);
    // make address and port could reuse.
    if((setsockopt(serfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&reuse, l) == -1) &&
            (setsockopt(serfd, SOL_SOCKET, SO_REUSEPORT, (const void *)&reuse, l) == -1))
    {
        HandleError("setsockopt failed\n");
    }
    set_noblock(serfd);
    listen(serfd, BACKLOG);
    return serfd;
}

/**
 * @brief InfoServer::accept_epoll
 * @param listen_sock
 * @return
 * server loop to accept client connection and data. If client not send data a long time,
 * it will reset the connection.
 */
int InfoServer::accept_epoll(int listen_sock, MEM *mem)
{
    struct epoll_event events[EVENTSMAX], ev;
    int nfds, clifd;
    bzero(&ev, sizeof(ev));
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    bzero(&cliaddr, len);
    char buff[1024] = {0};
    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1)
    {
        HandleError("epoll_ctl add listen_sock error\n");
    }
    for(;;)
    {
        nfds = epoll_wait(epollfd, events, EVENTSMAX, EPOLLTIMEOUT);
        if(nfds == 0)
        {
            // fprintf(stderr,"timeout\n");
            //EPOLLTIMEOUT is coming, reset all connections.
            delete_unnormal_conn();
            continue;
        }
        if(errno == EINTR || errno == EAGAIN)
            continue;
        if(nfds == -1)
        {
            HandleError("epoll_wait error\n");
        }
        for(int i = 0; i < nfds; ++i)
        {
            if(events[i].data.fd == listen_sock)
            {
label:
                clifd = accept(listen_sock, (struct sockaddr *)&cliaddr, &len);
                if(errno == EINTR || errno == EAGAIN)
                    goto label;
                if(clifd == -1){
                    fprintf(stderr, "%s\n", strerror(errno));
                    if(errno == ENOMEM || errno == ENOBUFS)
                    {
                        HandleError("accept error\n");
                    }

                }
                set_noblock(clifd);
                ev.data.fd = clifd;
                ev.events = EPOLLIN;
                if(epoll_ctl(epollfd, EPOLL_CTL_ADD, clifd, &ev) == -1)
                    HandleError("epoll_ctl add client fd failed\n");
                fprintf(stderr, "Record conn\n");
                //record client information and time.
                record_conn(cliaddr, clifd);
                //reset some clients which timeout.
                delete_unnormal_conn();
            }
            else if(events[i].events && EPOLLIN)
            {
                //check data completion
REREAD:
                int size = read(events[i].data.fd, buff, sizeof(buff)/sizeof(char));
                if(errno == EINTR)
                    goto REREAD;
                if(size){
                    bool header = false, complete = false;
                    int datalen = 0;
                    struct sockaddr_in addr;
                    bzero(&addr, sizeof(addr));
                    socklen_t addrlen = sizeof(addr);
                    getpeername(events[i].data.fd, (struct sockaddr*)&addr, &addrlen);
                    SingleConn sclient(addr, events[i].data.fd);
                    //check header and complete
                    check_header_complete(buff, size, datalen, header, complete);
                    if(complete && header){
                        //recv all data
                        mem->pushData(buff, size);
                        goto Lable2;
                    }
                    else if(!complete && header)
                    {
                        //recv header but data imcomplete
                        FINDCLIENT(sclient);
                        if(it != AllConns.end())
                        {
                            it->sgc.recvHeader(datalen);
                            // put data into client container.
                            int com = it->sgc.fillIntoData(buff, size);
                            if(com){
                                // data is full, put it to mem
                                it->sgc.pushIntoMem(mem);
                            }
                            goto Lable2;
                        }
                    }
                    else if(!header){
                        // recv data is not header
                        FINDCLIENT(sclient);
                        if(it != AllConns.end())
                        {
                            // this connect have recved header.
                            if(it->sgc.hadRecvHeader())
                            {
                                // fill into container until data complete
                                int com = it->sgc.fillIntoData(buff, size);
                                if(com == 1){
                                    // data is complete
                                    it->sgc.pushIntoMem(mem);
                                    goto Lable2;
                                }else{
                                    goto Lable2;
                                }
                            }else{
                                // data is malicious
                                Log(WARN, "Recved malicious data from %s .", inet_ntoa((it->sgc).cliaddr.sin_addr));
                                delete_special_conn(events[i].data.fd);
                            }
                        }
                    }
                }
                bzero(buff, sizeof(buff)/sizeof(char));
                epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &(events[i]));
                //Delete normal client in AllConns map.
                delete_normal_conn(events[i].data.fd);
                close(events[i].data.fd);
                //check all connections and remove unnormal connections.
Lable2:
                delete_unnormal_conn();
            }
        }
    }
    close(epollfd);
    return 1;
}

/**
 * @brief InfoServer::record_conn
 * @param cliaddr
 * @param clifd
 * record all clients and time to Allconn
 */
void InfoServer::record_conn(struct sockaddr_in cliaddr, int clifd)
{
    time_t conn_time = time(NULL) - ComTime;
    //SingleConn climap(cliaddr, clifd);
    fprintf(stderr,"%ld", conn_time);
    SC cli(cliaddr, clifd, conn_time);
    AllConns.push_front(cli);
}

/**
 * @brief InfoServer::delete_unnormal_conn
 * if connection over 8s , server send RST to client then close it.
 */
void InfoServer::delete_unnormal_conn()
{
    time_t secs = time(NULL) - ComTime;
    // fprintf(stderr, "%u", AllConns.size());
    for(it = AllConns.begin(); it != AllConns.end();)
    {
        if(EPOLLTIMEOUT/1000 <= (secs - it->t_time))
        {
            fprintf(stderr, ">>>>>>%ld", secs - it->t_time);
            struct epoll_event ev;
            ev.data.fd = it->sgc.clifd;
            ev.events = EPOLLIN;
            epoll_ctl(epollfd, EPOLL_CTL_DEL, it->sgc.clifd, &ev);
            send_RST(it->sgc.clifd);
            it = AllConns.erase(it);
        }else{
            ++it;
        }
    }

}

/**
 * @brief InfoServer::delete_special_conn
 * @param clifd
 * it recved data is not complete, then send RST to client.
 */
void InfoServer::delete_special_conn(int clifd)
{
    struct sockaddr_in cliaddr;
    socklen_t cli_len = sizeof(cliaddr);
    getpeername(clifd, (struct sockaddr *)&cliaddr, &cli_len);
    SingleConn cli(cliaddr, clifd);
    for(it = AllConns.begin(); it != AllConns.end();)
    {
        if(it->sgc == cli)
        {
            it = AllConns.erase(it);
            send_RST(clifd);
            break;
        }else{
            ++it;
        }
    }
}

/**
 * @brief InfoServer::delete_normal_conn
 * @param clifd
 * delete normal connection and erase from AllConns.
 */
void InfoServer::delete_normal_conn(int clifd)
{
    struct sockaddr_in cliaddr;
    bzero(&cliaddr, sizeof(cliaddr));
    socklen_t len = sizeof(cliaddr);
    getpeername(clifd, (struct sockaddr *)&cliaddr, &len);
    SingleConn client(cliaddr, clifd);
    for(it = AllConns.begin(); it != AllConns.end();)
    {
        if(it->sgc == client){
            it = AllConns.erase(it);
            break;
        }else{
            ++it;
        }
    }
}
