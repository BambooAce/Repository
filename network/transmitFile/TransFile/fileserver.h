#ifndef FILESERVER_H
#define FILESERVER_H
class FileServer
{
public:
    FileServer();
    ~FileServer();
    void init(int iport);
    void select_run(int listen_sock);
    void poll_run(int listen_sock);
    void epoll_run(int listen_sock);
private:
    int serverfd;
    int port;
};

#endif // FILESERVER_H
