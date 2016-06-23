#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <strings.h>
int main()
{
	int serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd < 0)
	{
		return -1;
	}
	struct sockaddr_in serveraddr;
	serveraddr.sin_port = htons(5050);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(serverfd, (const struct sockaddr*)&serveraddr, sizeof(serveraddr));
	listen(serverfd, 20);
	struct sockaddr_in  clientaddr;
	socklen_t len = sizeof(struct sockaddr_in);
	while (1)
	{
		int clientfd = accept(serverfd, (struct sockaddr*)&clientaddr, &len);
		char a[40];
		char  b[11];
		char c[11];
		bzero(a,13);
		bzero(b,11);
		bzero(c,11);
		write(1,"OK\n",3);
		sleep(2);
		read(clientfd, a, 40);
		//read(clientfd, b,11);
		//read(clientfd, c,11);
		printf("%s\n",a);
		//printf("%s\n",b);
		//printf("%s\n",c);
		close(clientfd);
	}
	close(serverfd);
	return 0;
}





