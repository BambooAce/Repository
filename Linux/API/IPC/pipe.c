#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int client(int, int);
int server(int, int);
int main()
{
	int pipefd[2], pipe2fd[2];
	pid_t pid;
	if(pipe(pipefd) == -1)
	{
		fprintf(stderr, "PIPE error!\n");
		exit(-1);
	}
	if(pipe(pipe2fd) == -1)
	{
		fprintf(stderr, "PIPE error!\n");
		exit(-1);
	}
	pid = fork();
	if(pid == -1)
	{
		fprintf(stderr, "Fork error!\n");
		exit(-1);
	}
	else if(pid == 0)
	{
		close(pipefd[0]);
		close(pipe2fd[1]);
		client(pipefd[1], pipe2fd[0]);
		exit(0);
	}else{
		close(pipefd[1]);
		close(pipe2fd[0]);
		server(pipefd[0], pipe2fd[1]);
		wait(NULL);
	}
	return 0;
}

int server(int readfd, int writefd)
{
	fd_set rfd;
	FD_ZERO(&rfd);
	char buff[256] = {0};
	for(;;)
	{
		FD_SET(readfd, &rfd);
		int ready = select(readfd+1, &rfd, NULL, NULL, NULL);
		if(ready == -1)
		{
			fprintf(stderr, "Select error!\n");
			exit(-1);
		}
		if(ready)
		{
			if(FD_ISSET(readfd, &rfd))
			{
				size_t readn;
				if( (readn = read(readfd, buff, 256)) > 0 )
				{
					write(writefd, buff, readn);
				}
				memset(buff, 0, 256);
			}
		
		}
	}
}

int client(int writefd, int readfd)
{
	fd_set rfd;
	FD_ZERO(&rfd);
	char wbuff[256] = {0};
	char rbuff[256] = {0};
	for(;;)
	{
		FD_SET(STDIN_FILENO, &rfd);
		FD_SET(readfd, &rfd);
		int ready = select(readfd+1, &rfd, NULL, NULL, NULL);
		if(ready == -1)
		{
			fprintf(stderr, "Select error!\n");
			exit(-1);
		}
		if(ready)
		{
			if(FD_ISSET(STDIN_FILENO, &rfd))
			{	
				printf("1");
				size_t readn;
				if((readn = read(STDIN_FILENO, wbuff, 256)) > 0)
				{
					write(writefd, wbuff, readn);
				}else{
					break;
				}
				memset(wbuff, 0, 256);
			}
			if(FD_ISSET(readfd, &rfd))
			{
				printf("2");
				size_t readn = read(readfd, rbuff, 256);
				if(readn)
					write(STDOUT_FILENO, rbuff, readn);
				memset(rbuff, 0, 256);
			}
			printf("3");
		}
	}
}

