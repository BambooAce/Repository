#include "Csocket/Csocket.h"
int main()
{
	Csocket serverfd;
	CreateServer(serverfd, 3333);
	sockaddr_in client;
	serverfd.Accept(client);
	
	return 0;
}
