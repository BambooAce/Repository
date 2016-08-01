#ifndef CSOCKET_H
#define CSOCKET_H
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
typedef struct sockaddr SA;
class Csocket
{
public:
    Csocket();
    ~Csocket();
    int Socket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
    sockaddr_in Setsockaddr(short port, std::string IP = "");
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
};

#endif // CSOCKET_H
