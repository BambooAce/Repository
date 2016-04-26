#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
//
//
// *************************************************
// After select or pselect function all fd_set will set NULL !!!
// so we must restore its fd_set. Fristly, we should
// make a copy to fd_set.
// And the time struct will set empty, after select function,
// but it`s not for pselect function.
// *************************************************
//
#define ERROR(CALL) { \
    perror(#CALL "error"); \
    fprintf(stderr, "%s\n", strerror(errno)); \
    exit(-1); \
    }
#define MAXLOG 20
void dokillchild(int signum)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
    {

    }
    return;
}

int main(void)
{
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd < 0)
        ERROR(socket);
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8000);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(serverfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
        ERROR(bind);
    if (-1 == listen(serverfd, MAXLOG))
        ERROR(listen);
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    signal(SIGCHLD, dokillchild);
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    struct timespec tv;
    tv.tv_sec = 15;
    tv.tv_nsec = 1050;
    while(1)
    {
        int clientfd = 0;
        if ((clientfd = accept(serverfd, (struct sockaddr *)&clientaddr, &len)) > 0)
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                ERROR(fork);
            }
            else if (0 == pid)
            {
                printf("get a connection\n");
                fd_set rfds;
                FD_ZERO(&rfds);
                FD_SET(clientfd, &rfds);
                FD_SET(fileno(stdin), &rfds);
                int maxfd = clientfd > fileno(stdin)? clientfd:fileno(stdin);
                while(1)
                {
                    int Ready = pselect(maxfd+1, &rfds, NULL, NULL, &tv, NULL);
                    if (Ready < 0)
                    {
                        ERROR(select);
                    }

                    else if (Ready == 0)
                    {
                        fprintf(stderr,"time out\n");
                        FD_ZERO(&rfds);
                        FD_SET(clientfd, &rfds);
                        FD_SET(fileno(stdin), &rfds);
                        continue;
                    }
                    else{
                        if (FD_ISSET(fileno(stdin), &rfds))
                        {
                            memset(buff, 0, sizeof(buff));
                            size_t readed = read(fileno(stdin), buff, sizeof(buff));
                            if (readed > 0 )
                            {
                                write(clientfd, buff, readed);
                            }
                        }
                        if (FD_ISSET(clientfd, &rfds))
                        {
                            memset(buff, 0, sizeof(buff));

                            size_t read_ = read(clientfd, buff, sizeof(buff));
                            if (read_)
                            {
                                write(1, buff, read_);
                            }
                            else if (read_ == 0)
                            {
                                close(clientfd);
                                break;
                            }
                        }

                    }
                    FD_ZERO(&rfds);
                    FD_SET(clientfd, &rfds);
                    FD_SET(fileno(stdin), &rfds);
                }
                exit(0);
            }else{

                continue;
            }

        }
    }
    close(serverfd);
    return 0;
}



















