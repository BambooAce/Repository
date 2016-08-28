#include "Csocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#ifdef LINUX
#include <netinet/in.h>
#endif
#define BACKLOG 1024
inline void handle_error(std::string msg)
{
	msg += " function fail !\n";
	perror(msg.c_str());
#ifndef LINUX
	WSACleanup();
#endif
	exit(-1);
}

Csocket::Csocket(int type)
{
#ifndef LINUX
	int n;
	WSADATA wd;  
    n = WSAStartup(MAKEWORD(1, 1), &wd);  
    if (n)  
    {  
		handle_error("Init");  
    }
#endif
	sockfd = socket(AF_INET, type, 0);
	if(INVALID_SOCKET == sockfd)
	{
		handle_error("Socket");
	}
}

bool Csocket::AddrInit(const int post, const char* addr)
{
	SetSockfd();
	memset(&(this->scr), 0, sizeof(scr));
	scr.sin_family = AF_INET;
	scr.sin_port = htons(post);
	if(strcmp(addr, "Any") == 0)
	{
#ifdef LINUX
		scr.sin_addr.s_addr = htonl(INADDR_ANY);
#else
		scr.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
#endif
		return true;
	}
#ifdef LINUX
	if(!inet_pton(AF_INET, addr, &scr.sin_addr))
	{
		return false;
	}
#else
	scr.sin_addr.S_un.S_addr = inet_addr(addr);
#endif
	return true;
}

void Csocket::SetSockfd()
{
	struct linger lig;
	lig.l_onoff = true;
	lig.l_linger = 50;
	setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (const char *)&lig, sizeof(lig));
}

void Csocket::Listen()
{
	int reuse = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));
	if( -1 == listen(sockfd, BACKLOG))
	{
		Close();
		handle_error("Listen");
	}
}

void Csocket::Bind()
{
	int len = sizeof(scr);
	if(-1 == bind(sockfd, (const sockaddr *)&scr, len))
	{
		Close();
		handle_error("Sockaddr is NULL or Bind");
	}
}

SOCKET Csocket::Accept(sockaddr_in & clientaddr)
{
	unsigned int addrlen = sizeof(clientaddr);
	SOCKET clientfd = accept(sockfd, (sockaddr*)&clientaddr, &addrlen);
	if(clientfd == -1)
	{
		Close();
		handle_error("Accept");
	}
	return clientfd;
}

void Csocket::Connect()
{
	if ( -1 == connect(sockfd, (const sockaddr*)&scr, sizeof(scr)))
	{
		Close();
		handle_error("Connect");
	}
}

void Csocket::Close()
{
#ifdef LINUX
	close(sockfd);
#else
	closesocket(sockfd);
#endif
}

Csocket::~Csocket(void)
{
	Close();
#ifndef LINUX
	WSACleanup();
#endif
}

bool CreateClient(Csocket &client, const int post, const char* addr)
{
	if(client.AddrInit(post, addr))
	{
		client.Connect();
		return true;
	}else
		return false;
}

bool CreateServer(Csocket &server, const int post, const char* addr)
{
	if(server.AddrInit(post, addr))
	{
		server.Bind();
		server.Listen();
		return true;
	}else
		return false;
}

