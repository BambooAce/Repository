#ifndef FILESERVER_H
#define FILESERVER_H


class FileServer
{
public:
    FileServer();
    ~FileServer();
    void init(int iport);
    void thread_run();
private:
    int serverfd;
    int port;
};

#endif // FILESERVER_H
