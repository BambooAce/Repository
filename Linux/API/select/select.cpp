#include <sys/select.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
/*
 *Select function used to set time for block IO function, its time unuseful next time.
 *Pselect function used to set time for block IO function but its time always useful.
 */
int main()
{
    fd_set rfds;
    struct timeval tv;
    struct timespec ts;
    ts.tv_sec = 2;
    ts.tv_nsec = 500;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    /* Wait up to five seconds. */
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    //retval = select(1, &rfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */
    retval = pselect(1, &rfds, NULL, NULL, &ts, NULL);
    if (retval == -1)
        perror("select()");
    else if (retval)
        printf("Data is available now.\n");
    /* FD_ISSET(0, &rfds) will be true. */
    else
    {
        printf("No data within five seconds.\n");
        //printf("Select: The time set is %ld, %ld\n",tv.tv_sec, tv.tv_usec);
        printf("Pselect: The time set is %ld, %ld\n",ts.tv_sec, ts.tv_nsec);
    }
    return 0;
}
