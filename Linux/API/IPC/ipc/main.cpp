#include <iostream>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <unistd.h>


using namespace std;

int main(int argc, char *argv[])
{
    mqd_t a = mq_open("/hello", O_CREAT);
    cout << "Hello World!" << endl;
    while(1)
    {

        sleep(2);
    }
    return 0;
}
