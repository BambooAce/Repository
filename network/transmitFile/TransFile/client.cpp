#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "fileclient.h"
#include "handlestream.h"
bool Isconn = false;
std::string header = "";

#define FormatError(error) do{ \
    fprintf(stderr, error); \
    _exit(1); \
    }while(0)

typedef struct {
    FileClient *fclient;
    char *mge;
}ARGS;

void *gotoconn(void *arg);
void *gotogetMD5(void *arg);

int parse_format(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    int mode = parse_format(argc, argv);
    FileClient *client = new FileClient(argv[3], 7682);
    ARGS arg = {client, argv[2]};
    pthread_t conn, cal;
    pthread_create(&conn, NULL, gotoconn, &arg);
    if(!mode){
        pthread_create(&cal, NULL, gotogetMD5, &arg);
        pthread_join(cal, NULL);
    }else{
        std::string filename = parseFilename(argv[2]);
        header = client->setHeader(mode, filename);
    }
    pthread_join(conn, NULL);
    if(Isconn)
    {
        client->sendHeader(header);
        sleep(10);
    }
    return 0;
}

void *gotoconn(void *arg)
{
    ARGS *cliarg = (ARGS *)arg;
    Isconn = cliarg->fclient->connServer();
    return 0;
}

void *gotogetMD5(void *arg)
{
    ARGS *cliarg = (ARGS *)arg;
    std::string md5 = calMD5(cliarg->mge);
    int size = getSize(cliarg->mge);
    std::string filename = parseFilename(cliarg->mge);
    header = cliarg->fclient->setHeader(0, filename, size, md5);
	return 0;
}

int parse_format(int argc, char *argv[])
{
    if(argc != 4)
    {
        FormatError("Format : fileclient PUT/GET filename serverIP\n");
    }
    int mode = 0;
    if((strcmp(argv[1], "PUT") == 0) || (strcmp(argv[1], "put") == 0))
        mode = 0;
    else if((strcmp(argv[1], "GET") == 0) || (strcmp(argv[1], "get") == 0))
        mode = 1;
    else{
        FormatError("Format : fileclient PUT/GET filename serverIP\n");
    }
    if((access(argv[2], F_OK) == -1) && !mode)
    {
        FormatError("File not exist\n");
    }else if((access(argv[2], F_OK) == 0) && mode)
    {
        FormatError("File exist\n");
    }
    return mode;
}
