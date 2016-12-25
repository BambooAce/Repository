#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#define PORT 6666
#define MAXSIZE 1024
void str_ser(int);
int main()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serveraddr, clientaddr;
	//serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_aton("192.168.1.123",&serveraddr.sin_addr);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	bind(fd,(const struct sockaddr *)&serveraddr, sizeof(serveraddr));
	listen(fd, 5);
	socklen_t len = sizeof(clientaddr);
	for(;;)
	{
		int clientfd = accept(fd, (struct sockaddr *)&clientaddr, &len);
		if(fork() == 0)
		{
			close(fd);
			str_ser(clientfd);
			exit(0);
		}
		close(clientfd);
	}
}

void str_ser(int clientfd)
{
	char buff[MAXSIZE];
	size_t n;
	bzero(buff, MAXSIZE);
	while( (n = read(clientfd, buff, MAXSIZE)) > 0)
	{
	
			write(clientfd, buff, n);
			bzero(buff, MAXSIZE);
	}
	if(n <= 0)
		fprintf(stderr, "read error\n");

}

