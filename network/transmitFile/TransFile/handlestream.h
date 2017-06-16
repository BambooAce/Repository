#ifndef HANDLESTREAM_H
#define HANDLESTREAM_H
#include <string>

std::string calMD5(char *path);
int readn(int fd, char * buf, int size);
int writen(int fd, char * buf, int size);
std::string getIPaddr(std::string url);

#endif // HANDLESTREAM_H
