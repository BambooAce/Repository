#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
const int MAXSIZE = 1024;
void select_func(int sockfd);
int main()
{
		int sockfd = socket(AF_INET,SOCK_STREAM,0);
		if (sockfd == -1)
		{
			perror("Socket error\n");
			exit(-1);
		}
		struct sockaddr_in sersock;
		socklen_t len = sizeof(sersock);
		bzero(&sersock,len);
		sersock.sin_family = AF_INET;
		//inet_pton(AF_INET,"",&sersock.sin_addr);
		sersock.sin_addr.s_addr = htonl(INADDR_ANY);
		sersock.sin_port = htons(3333);
		if (bind(sockfd, (struct sockaddr*)&sersock, len) == -1)
		{
			perror("Bind fail\n");
			exit(-1);
		}
		if(listen(sockfd, 100) == -1)
		{
			perror("Listen fail\n");
			exit(-1);
		}
		select_func(sockfd);
		return 0;
}

void select_func(int listenfd)
{
	struct sockaddr_in clientAddr;    
    socklen_t addrLen;    
    int maxfd = listenfd;    
    fd_set rset;    
    fd_set allset;    
    FD_ZERO(&rset);    
    FD_ZERO(&allset);    
    FD_SET(listenfd, &allset);    
     
    int client[FD_SETSIZE];  //用于保存已连接的客户端套接字   
    for (int i = 0; i < FD_SETSIZE; ++i)    
        client[i] = -1;    
    int maxi = 0;   //用于保存最大的不空闲的位置, 用于select返回之后遍历数组    
    
    while (1)    
    {    
		printf("fdk\n");
		//FD_SET(listenfd, &allset); 
        rset = allset;    
        int nReady = select(maxfd+1, &rset, NULL, NULL, NULL);    
        if (nReady == -1)    
        {    
            if (errno == EINTR)    
                continue;    
            exit(-1);  
        }    
        //nReady == 0表示超时, 但是此处是一定不会发生的    
        else if (nReady == 0)    
            continue;    
    
        if (FD_ISSET(listenfd, &rset))    
        {    
			printf("OKOKOK\n");
            addrLen = sizeof(clientAddr);    
            int connfd = accept(listenfd, (struct sockaddr *)&clientAddr, &addrLen);    
            if (connfd == -1)    
                exit(-1);   
    
            int i;    
            for (i = 0; i < FD_SETSIZE; ++i)    
            {    
                if (client[i] < 0)    
                {    
                    client[i] = connfd;    
                    if (i > maxi)    
                        maxi = i;    
                    break;    
                }    
            }    
            if (i == FD_SETSIZE)    
            {    
                
                exit(EXIT_FAILURE);    
            }     
            FD_SET(connfd, &allset);    
            if (connfd > maxfd)    
                maxfd = connfd;    
    
            if (--nReady <= 0)    
                continue;    
        }    
		printf("ok\n");
        /**如果是已连接套接口发生了可读事件**/    
        for (int i = 0; i <= maxi; ++i)
        {    
            if ((client[i] != -1) && FD_ISSET(client[i], &rset))    
            {    
                char buf[512] = {0};    
                int readBytes = read(client[i], buf, sizeof(buf));    
                if (readBytes == -1)    
                    exit(-1);    
                else if (readBytes == 0)    
                {    
                    
                    FD_CLR(client[i], &allset);    
                    close(client[i]);    
                    client[i] = -1;    
                }      
                sleep(1);     
                if (write(client[i], buf, readBytes) == -1)    
                    exit(-1);    
    
                if (--nReady <= 0)    
                    break;    
            }    
        }
    }   

}
