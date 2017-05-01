#ifndef FILESERVER_H
#define FILESERVER_H
#include <map>
#include <string>
class FileServer
{
public:
    FileServer();
    ~FileServer();
    void init(int iport);
    void thread_run();
    void select_run();
    void poll_run();
    void epoll_run();
private:
    int serverfd;
    int port;
    std::map<std::string, std::string> mdfilemap;
    void getmd5(std::string);
};

#endif // FILESERVER_H
