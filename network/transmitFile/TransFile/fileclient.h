#ifndef FILECLIENT_H
#define FILECLIENT_H
#include "msgitem.h"
#include <string>
class FileClient
{
public:
    FileClient(char * url, int port);
    ~FileClient();
    std::string setHeader(int mode, std::string filename, std::string filesize, std::string md5);
    void sendFile(char *filepath);
    void recvFile(char *filepath);
private:
    int clifd;
    MsgItem msg;
    void init();
};

#endif // FILECLIENT_H
