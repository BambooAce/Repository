#include "Csocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
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
#ifdef LINUX
	unsigned int addrlen = sizeof(clientaddr);
label:
    struct timeval tval;
    tval.tv_sec = 15;
    tval.tv_usec = 300;
    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(sockfd, &rfd);
    if (select(sockfd+1, &rfd, NULL, NULL, &tval) > 0)
    {
#else
        int addrlen = sizeof(clientaddr);
#endif
        SOCKET clientfd = accept(sockfd, (sockaddr*)&clientaddr, &addrlen);
        if(clientfd == -1)
        {
            Close();
            handle_error("Accept");
        }
        return clientfd;
#ifdef LINUX
    }else
    {
        printf("Timeout\n");
        goto label;
    }
#endif
}

void Csocket::Connect()
{
#ifdef LINUX
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
#endif
	if ( -1 == connect(sockfd, (const sockaddr*)&scr, sizeof(scr)))
	{
		Close();
		handle_error("Connect");
	}
#ifdef LINUX
    struct timeval tval;
    tval.tv_sec = 15;
    tval.tv_usec = 300;
	fd_set wfd;
	FD_ZERO(&wfd);
	FD_SET(sockfd, &wfd);
    if(select(sockfd + 1, NULL, &wfd, NULL, &tval) == 1)
    {
        int so_error;
        socklen_t len = sizeof so_error;
        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len);
        if(so_error == 0)
        {
            printf("Connect success!\n");
        }else
        {
            printf("Connect fail!\n");
            exit(1);
        }
    }else{
        printf("Connect fail! Time out \n");
        exit(1);
    }
#endif
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

void select_func(Csocket& listenfd)
{
	SOCKET lisfd = listenfd.sockfd;
	fd_set rset, allset;
	FD_ZERO(&rset);
	FD_ZERO(&allset);
	FD_SET(lisfd, &allset);
	int client[1024];
	for(int i = 0; i < 1024; ++i)
	{
		client[i] = -1;
	}
	int maxi = -1;
	SOCKET maxfd;
	maxfd = lisfd;
	int nready, n;
	sockaddr_in clientaddr;
#ifdef LINUX
	unsigned int addrlen = sizeof(clientaddr);
#else
	int addrlen = sizeof(clientaddr);
#endif
	memset(&clientaddr, 0, addrlen);
	for(;;)
	{
		struct timeval tval;
		tval.tv_sec = 60;
		tval.tv_usec = 60;
		rset = allset;
		nready = select(maxfd + 1, &rset, 0, 0, &tval);
		if( SOCKET_ERROR == nready)
		{
			if(errno == EINTR)
			{
				continue;
			}
			listenfd.Close();
			handle_error("Select");
		}
		else if(nready == FD_SETSIZE)
		{
			listenfd.Close();
			handle_error("Too many client");
		}
		else if(0 == nready)
		{
			fprintf(stderr, "Time out\n");
			continue;
		}
		else {
		
			if(FD_ISSET(lisfd, &rset))
			{
				SOCKET clientfd = listenfd.Accept(clientaddr);
				for (n = 0; n < 1024; ++n)
				{
					if(-1 == client[n])
					{
						client[n] = clientfd;
						break;
					}
				}
				if(n == 1023)
				{
					listenfd.Close();
					handle_error("Too many client");
				}
				FD_SET(clientfd, &allset);
				maxi = maxi > n? maxi:n;
				maxfd = maxfd > clientfd? maxfd:clientfd;
				if(--nready <= 0)
					continue;
			}
			for(int m = 0; m < maxi; ++m)
			{
				if ((client[m] > -1)&&FD_ISSET(client[m], &rset))
				{
					
				
				}
			}
		
		}
	}
}
