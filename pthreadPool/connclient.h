#include <netinet/in.h>
#include <time.h>
typedef struct clientInfo{
    int clifd;  // file descriptor
    in_addr_t ipaddr; // ipv4 address
}CLIENTINFO;

typedef struct{
    CLIENTINFO cinfo; //client information.
    time_t connedtime; //connected time.
    int flag;    //whether send header information.
}CONNCLIENT;
