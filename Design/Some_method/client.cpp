#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
	int clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if ( clientfd < 0 )
	{
		return -1;
	}
	struct sockaddr_in serveraddr;
	serveraddr.sin_port = htons(5050);
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &serveraddr.sin_addr);
	connect(clientfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr));
	
	write(clientfd, "Helloworld40",13);
	write(clientfd, "hihihihi30",11);
	write(clientfd, "bibibibi20",11);
	close(clientfd);
	sleep(5);
	return 0;


}
