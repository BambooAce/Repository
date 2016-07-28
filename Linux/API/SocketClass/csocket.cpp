#include "csocket.h"
#include "csocketex.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
Csocket::Csocket():_sockfd(-1),_port(0),strIP(""),len(sizeof(SA))
{

}
Csocket::~Csocket()
{
    this->Close();
}

int Csocket::Socket(int domain, int type, int protocol)
{
    this->_sockfd = socket(domain, type, protocol);
    if(this->_sockfd < 0){
        throw SocketEX();
    }
    return _sockfd;
}

SA Csocket::Setsockaddr(short port, std::string IP)
{
    struct sockaddr_in addrin;
    addrin.sin_family = AF_INET;
    if(strcmp(IP.c_str(), "") == 0)
        addrin.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        inet_pton(AF_INET, IP.c_str(), addrin.sin_addr);
    addrin.sin_port = htons(port);
    return (SA)addrin;
}

void Csocket::Bind(const SA *sa)
{
    int a = bind(_sockfd, sa, len);
    if(0 != a)
    {
       throw BindEX();
    }
}

void Csocket::Listen(int backlog)
{
    if(0 != listen(_sockfd, backlog))
        throw ListenEX();
}

int Csocket::Accept(SA *sa)
{
    int clientfd = accept(_sockfd, sa, &len);
    if(clientfd <= 0)
    {
        throw AcceptEX();
    }else
        return clientfd;

}

void Csocket::Connect(SA *sa)
{
    if(0 != connect(_sockfd, sa, len))
        throw ConnectEX();
}

void Csocket::Setsockopt(int level, int optname, const void * optval, socklen_t optlen)
{
    if (0 != setsockopt(_sockfd, level, optname, optval, optlen))
        throw SockoptEX();
}

void Csocket::Close()
{
    if(_sockfd>0)
        close(_sockfd);
}

