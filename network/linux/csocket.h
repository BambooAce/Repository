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
    bool bTCP();
    //socket file option. SOL_SOCKET
    bool SetNONBlock();
    bool SetSockBroadcast();
    bool SetSockKeepAlive();
    bool SetSockLingen();
    bool SetSockBuf(int type, int size);
    bool SetSockLowat(int type, int size);
    bool SetSockTimeout(int type, int time);
    bool SetSockReuseaddr();
    //IPv4 options IPPROTO_IP
    bool SetSockIPHead();
    //TCP options IPPROTO_TCP
    bool SetSockMSS();
    bool SetSockNagle();
    int Shutdown(int how);
    int GetFd();
private:
    int sockfd;
    bool tcp;
};
typedef void (*RunFun)(Csocket &client, char * addr, unsigned short port);
void CreateUDPServer(Csocket &server, unsigned short port);
void CreateTCPServer(Csocket &server, unsigned short port);
void CreateUDPClient(Csocket &client, char *addr, unsigned short port, RunFun runfun);
void CreateTCPClient(Csocket &client, char *addr, unsigned short port, RunFun runfun, int timeout = 0);
void ShowClient(int clientfd);
#endif // CSOCKET_H

