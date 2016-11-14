#include "../csocket.h"
#include <netinet/in.h>
int main()
{
    Csocket server;
    server.Socket();
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(6666);
    serveraddr.sin_addr = htonl()

    server.Bind();
    return 0;
}
