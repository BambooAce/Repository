#ifndef _CSOCKET_H
#define _CSOCKET_H
#ifdef LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#else
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif
class Csocket
{
	friend void select_func(Csocket &server);
public:
	Csocket(int type = SOCK_STREAM);
	~Csocket(void);	
	bool AddrInit(const int post, const char* addr = "Any");
	void Listen();
	SOCKET Accept(sockaddr_in & clientaddr);
    void Connect();
	void Bind();
	void Close();
	Csocket(const Csocket &cs);
	Csocket &operator=(const Csocket &cs);
private:
	SOCKET sockfd;
	sockaddr_in scr;
	void SetSockfd();
};
bool CreateClient(Csocket &client, const int post, const char* addr);
bool CreateServer(Csocket &server, const int post, const char* addr = "Any");

#endif
