#include "deamon.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

/**
 * @brief init_deamon
 * Init deamon close strandard input output and tty
 */
void init_deamon()
{
    int i;
    int cpid = fork();
    if(cpid > 0)
    {
        _exit(0);
    }
    else if(cpid == -1)
    {
        _exit(0);
    }
    if(setsid() == -1)
    {
        _exit(0);
    }
    int fd = open("/dev/tty", O_RDONLY);
    if(fd)
        close(fd);
    cpid = fork();
    if(cpid > 0)
        _exit(0);
    else if(cpid == -1)
        _exit(0);
    chdir("/");
    fd = open("/dev/tty", O_RDONLY);
    if(fd)
        close(fd);
    for(i = 0; i < 3; ++i)
        close(i);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_WRONLY);
    umask(0);
}
