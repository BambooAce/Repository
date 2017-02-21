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
#include <errno.h>
#define PORT 6666
#define MAXSIZE 1024
void str_cli(FILE *, int);
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
	connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	//FILE * fp = fopen("./aa","r");
	//str_cli(fp, fd);
	sleep(60);
	close(fd);
	sleep(1000);
	exit(0);
}

void str_cli(FILE * fp, int fd)
{
	char sendbuff[MAXSIZE], recvbuff[MAXSIZE];
	bzero(sendbuff, MAXSIZE);
	bzero(recvbuff,MAXSIZE);
	while (fgets(sendbuff, MAXSIZE, fp) != NULL)
	{
		write(fd, sendbuff, strlen(sendbuff));
		printf("hello\n");
		/*	if(read(fd, recvbuff, MAXSIZE) == 0)
		{	
			if(errno == ECONNRESET)
			{
				fprintf(stderr, "reconnect\n");		
			}
			fprintf(stderr, "server terminated!\n");
			exit(1);
		}*/
		fputs(recvbuff, stdout);
		bzero(sendbuff, MAXSIZE);
		bzero(recvbuff,MAXSIZE);
	}
}

