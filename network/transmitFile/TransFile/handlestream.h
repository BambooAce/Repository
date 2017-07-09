#ifndef HANDLESTREAM_H
#define HANDLESTREAM_H
#include <string>
#include <openssl/conf.h>
typedef struct {
    std::string respath;
    int port;
    std::string dbaddr;
    std::string dbname;
    std::string dbtable;
}INI;
std::string calMD5(char *path);
int readn(int fd, char * buf, int size);
int writen(int fd, char * buf, int size);
std::string getIPaddr(std::string url);
int getSize(char *path);
std::string parseFilename(char *path);
int parse_ini(char *inipath, struct INI * inistruct);
#endif // HANDLESTREAM_H
