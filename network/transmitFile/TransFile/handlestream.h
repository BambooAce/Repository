#ifndef HANDLESTREAM_H
#define HANDLESTREAM_H
#include <string>
class HandleStream
{
public:
    HandleStream();
    ~HandleStream();
    std::string calMD5(char *path);
    int readn(int fd, char * buf, int size);
    int writen(int fd, char * buf, int size);
};

#endif // HANDLESTREAM_H
