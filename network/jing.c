#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#define IP   "127.0.0.1"
#define PORT  8888
#define PROCESS_NUM 4
#define MAXEVENTS 64

static int
create_and_bind ()
{
    int fd = socket (PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton (AF_INET, IP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons (PORT);
    bind (fd, (struct sockaddr *) &serveraddr, sizeof (serveraddr));
    return fd;
}

static int
make_socket_non_blocking (int sfd)
{
    int flags, s;
    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror ("fcntl");
        return -1;
    }
    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
    {
        perror ("fcntl");
        return -1;
    }
    return 0;
}

void
worker (int sfd, int efd, struct epoll_event *events, int k, sem_t * sem)
{
    /* The event loop */
    struct epoll_event event;
    // struct epoll_event *events;
    efd = epoll_create (MAXEVENTS);
    if (efd == -1)
    {
        perror ("epoll_create");
        abort ();
    }
    int epoll_lock = 0;
    while (1)
    {
        int n, i;
        int s;
        event.data.fd = sfd;
        event.events = EPOLLIN;
        if (0 == sem_trywait (sem))
        {
            //拿到锁的进程将listen 描述符加入epoll
            if (!epoll_lock)
            {
                fprintf (stderr, "%d  >>>get lock\n", k);
                s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);
                if (s == -1)
                {
                    perror ("epoll_ctl");
                    abort ();
                }
                epoll_lock = 1;
            }
        }
        else
        {
            fprintf (stderr, "%d not lock\n", k);
            //没有拿到锁的进程 将lisfd 从epoll 中去掉
            if (epoll_lock)
            {
                fprintf (stderr, "worker  %d return from epoll_wait!\n", k);
                if (-1 == epoll_ctl (efd, EPOLL_CTL_DEL, sfd, &event))
                {
                    if (errno == ENOENT)
                    {
                        fprintf (stderr, "EPOLL_CTL_DEL\n");
                    }
                }
                epoll_lock = 0;
            }
        }
        //epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);
        // fprintf(stderr, "ok\n");
        //不能设置为-1  为了能让拿不到锁的进程再次拿到锁
        n = epoll_wait (efd, events, MAXEVENTS, 300);
        for (i = 0; i < n; i++)
        {
            if (sfd == events[i].data.fd)
            {
                /* We have a notification on the listening socket, which means one or more incoming connections. */
                struct sockaddr in_addr;
                socklen_t in_len;
                int infd;
                char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
                in_len = sizeof in_addr;
                while ((infd = accept (sfd, &in_addr, &in_len)) > 0)
                {
                    fprintf(stderr, "get one\n");
                    close (infd);
                }
            }
        }
        if (epoll_lock)
        {
            //这里将锁释放
            sem_post (sem);
            epoll_lock = 0;
            epoll_ctl (efd, EPOLL_CTL_DEL, sfd, &event);
        }
    }
}

int
main (int argc, char *argv[])
{
    int shmid;
    sem_t *acctl;
    //建立共享内存
    shmid = shmget (IPC_PRIVATE, sizeof (sem_t), 0600);
    acctl = (sem_t *) shmat (shmid, 0, 0600);
    //进程间信号量初始化   要用到上面的共享内存
    sem_init (acctl, 1, 1);
    int sfd, s;
    int efd;
    // struct epoll_event event;
    // struct epoll_event *events;
    sfd = create_and_bind ();
    if (sfd == -1)
    {
        abort ();
    }
    s = make_socket_non_blocking (sfd);
    if (s == -1)
    {
        abort ();
    }
    s = listen (sfd, SOMAXCONN);
    if (s == -1)
    {
        perror ("listen");
        abort ();
    }
    efd = 0;
    int k;
    for (k = 0; k < PROCESS_NUM; k++)
    {
        printf ("Create worker %d\n", k + 1);
        int pid = fork ();
        if (pid == 0)
        {
            struct epoll_event *events;
            events = calloc (MAXEVENTS, sizeof (struct epoll_event));
            worker (sfd, efd, events, k, acctl);
            break;
        }
    }
    int status;
    wait (&status);
    close (sfd);
    return EXIT_SUCCESS;
}
/*
 * 这里处理惊群 用到了进程的锁（信号量， 共享内存）， 根据试验的结果多个进程时accept接收客户端连接的效率并没有提高太多
 * 但是处理其他可读可写（非监听描述符）时， 要比单个进程要快很多。
*/

