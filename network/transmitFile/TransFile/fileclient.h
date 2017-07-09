#ifndef FILECLIENT_H
#define FILECLIENT_H
#include "msgitem.h"
#include <string>
#include <stdio.h>
class FileClient
{
public:
    FileClient(std::string url, int iport);
    ~FileClient();
    std::string setHeader(int mode, std::string &filename, int filesize = 0, std::string md5 = "");
    bool connServer();
    void sendHeader(std::string header);
    void sendFile(FILE *fp);
    void recvFile(FILE *fp);
private:
    int clifd;
    MsgItem msg;
    std::string urladd;
    int port;
};

#endif // FILECLIENT_H
