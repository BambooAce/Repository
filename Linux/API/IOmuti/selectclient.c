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
#include <signal.h>
void handle_signal(int sig);
void client_select(int clientfd);
int main(int argc, char * argv[])
{		
		if(argc != 2)
		{
			fprintf(stderr, "Like: ./client 192.168.1.3");
			exit(-1);
		}
		int clientfd = socket(AF_INET, SOCK_STREAM, 0);
		if (clientfd == -1)
		{
			perror("Socket fail\n");
			exit(-1);
		}
		signal(SIGALRM, handle_signal);
		struct sockaddr_in seraddr;
		seraddr.sin_family = AF_INET;
		seraddr.sin_port = htons(3333);
		inet_pton(AF_INET,argv[1],&seraddr.sin_addr);
		socklen_t len = sizeof(seraddr);
		alarm(10);
		if (connect(clientfd, (struct sockaddr*)&seraddr, len) == 0)
		{
			alarm(0);
			printf("connect ok\n");
			client_select(clientfd);
		}
		return 0;
}

void handle_signal(int sig)
{
	sig = 0;
	fprintf(stderr, "Time out , not connect server\n");
	exit(0);
}

void client_select(int clientfd)
{
	char buf[1024];
	bzero(buf, 1024);
	int fdin = fileno(stdin);
	int maxfd;
	fd_set rset;
	FD_ZERO(&rset);
	FD_ZERO(&rset);
	for(;;)
	{	
		FD_SET(fdin, &rset);
		FD_SET(clientfd, &rset);
		maxfd = fdin > clientfd ? fdin:clientfd;
		int ready = select(maxfd+1, &rset, NULL, NULL, NULL);
		if(ready == -1)
		{
			fprintf(stderr,"Select fail\n");
			continue;
		}
		else{
			if(FD_ISSET(fdin, &rset))
			{
				size_t n = read(0, buf, 1024);
				if (n == 0)
				{
					FD_CLR(fdin, &rset);
					continue;
				}
				write(clientfd, buf, n);
				
			}
			if(FD_ISSET(clientfd, &rset))
			{
				size_t n = read(clientfd, buf, 1024);
				if (n == 0)
				{
					fprintf(stderr, "Server is closed\n");
					close(clientfd);
					FD_CLR(clientfd, &rset);
					exit(1);
				}
				write(1, buf, n);
			}

			
		}
	
	}

}
