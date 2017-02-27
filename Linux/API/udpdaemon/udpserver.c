#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 4444
#define FDMAX 64
void initdaemon();
int main()
{
	#ifdef DAEMON
		initdaemon();
	#endif
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
        return -1;
    struct sockaddr_in server, client;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(fd, (const struct sockaddr *)&server, sizeof(struct sockaddr));
    socklen_t len  = sizeof(struct sockaddr);
    char buff[2048] = {0};
    while(1)
    {
        recvfrom(fd, buff, 2048, 0,(struct sockaddr *)&client, &len);
       // printf("%s send msg: %s \n", inet_ntoa(client.sin_addr), buff);
        sendto(fd, "OK", 2, 0, (const struct sockaddr *)&client, len);
    }
}
void initdaemon()
{
	pid_t pid = fork();
	if(pid > 0)
	{
		_exit(0);
	}
	else if(pid < 0)
	{
		_exit(1);
	}
	if(setsid() < 0)
		_exit(1);
	pid_t id = fork();
	if(id > 0){
		_exit(0);
	}
	else if(id < 0)
	{
		_exit(1);
	}
	int i = 0;
	chdir("/");
	for(i; i < FDMAX; ++i)
		close(i);
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);
}
