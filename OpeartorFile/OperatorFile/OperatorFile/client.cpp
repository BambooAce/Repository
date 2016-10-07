#include "Csocket/Csocket.h"
int main()
{
	Csocket clientfd;
	CreateClient(clientfd, 3333, "127.0.0.1");
	
	return 0;
}
