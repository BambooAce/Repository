#ifndef CSOCKET_H
#define CSOCKET_H
struct sockaddr_in;
class Csocket
{
public:
    Csocket();
    ~Csocket();
    bool Connect(char *addr, unsigned short port, int timeout=0);
    bool Listen();
    int Accept();
    bool Bind(struct sockaddr_in &seraddr);
    int Socket(bool TCP=true);
    bool SetNONBlock();
    bool SetSock();
    int GetFd();
private:
    int sockfd;

};

bool CreateUDPServer(Csocket server, unsigned short port);
bool CreateTCPServer(Csocket server, unsigned short port);
bool CreateUDPClient(Csocket client, char *addr, unsigned short port);
bool CreateTCPClient(Csocket client, char *addr, unsigned short port, int timeout = 0);
void ShowClient(int clientfd);
#endif // CSOCKET_H

