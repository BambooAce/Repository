#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <errno.h>
#include <signal.h>
#define PORT 6666
#define MAXSIZE 1024
void str_cli(int);
void *doit(int s)
{
	printf("NO NO\n");

}
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "./client IP\n");
		return 1;
	}
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	inet_pton(AF_INET, argv[1], &serveraddr.sin_addr);
//    char buff[256]={0};
//    fd_set rdf;
//    FD_ZERO(&rdf);
//    FD_SET(fd, &rdf);
	connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
//    int nready = select(fd+1, &rdf, NULL, NULL, 0);
//    if(nready)
//    {
//        if(FD_ISSET(fd, &rdf) && errno == ECONNREFUSED)
//        {
//            if(read(fd, buff, 256) <= 0)
//                printf("RST?\n");
//        }
//    }
	//FILE * fp = fopen("./aa","r");
	//str_cli(fp, fd);
	signal(SIGPIPE, doit);
    if(errno == ENETUNREACH)
    {
        printf("server can't reach\n");
    }
	str_cli(fd);
	sleep(600);
	close(fd);
	exit(0);
}

void str_cli(int fd)
{
	char sendbuff[MAXSIZE] = "hello world";
	while (1)
	{
		int n;
		if((n = write(fd, sendbuff, strlen(sendbuff))) > 0)
		{
			printf("hello\n");
		}
		else
		{
		
			printf("REST\n");
		if(errno == EPIPE)
		{
			printf("EPIPE\n");
		}
		}
		/*	if(read(fd, recvbuff, MAXSIZE) == 0)
		{	
			if(errno == ECONNRESET)
			{
				fprintf(stderr, "reconnect\n");		
			}
			fprintf(stderr, "server terminated!\n");
			exit(1);
		}*/
		sleep(1);
	}
}

