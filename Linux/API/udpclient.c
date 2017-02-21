#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 4444
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("./udpclient IP\n");
		return 1;
	}
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return 1;
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	if (!inet_pton(AF_INET, argv[1], (void *)&server.sin_addr))
	{
		printf("IP error\n");
		return -1;
	}
    char buff1[65535] = "sdfsdfsdf";
    char buff2[2048] = {0};
    socklen_t size = sizeof(struct sockaddr);
    while(1)
    {
        sendto(fd, buff1, 65535, 0, (const struct sockaddr *)&server, size);
        sleep(5);
        recvfrom(fd, buff2, 2048, 0, NULL, NULL);
        printf("recv : %s\n", buff2);
    }
	
}
