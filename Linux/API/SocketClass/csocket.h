#ifndef CSOCKET_H
#define CSOCKET_H
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
typedef struct sockaddr SA;
class Csocket
{
public:
    Csocket();
    ~Csocket();
    int Socket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
    SA Setsockaddr(short port, std::string IP = "");
    void Bind(const SA* sa);
    void Listen(int backlog);
    int Accept(SA * sa);
    void Connect(SA *sa);
    void Setsockopt(int level, int optname, const void *optval, socklen_t optlen);
    void Close();
private:
    int _sockfd;
    short _port;
    std::string strIP;
    const socklen_t len;
};

#endif // CSOCKET_H
