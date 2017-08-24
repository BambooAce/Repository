#include "connclient.h"

//client node in clients list
typedef struct clientnode{
    CONNCLIENT nclient;
    clientnode *next;
    clientnode *preclient;
}CLIENTNODE;

//Clientlist header
typedef struct{
    int size;
    CLIENTNODE * header;
}CLIENTLIST;

//handler connection function
typedef void (*HANDLER_CONN)(CLIENTINFO cinfo);

//create connect list
CLIENTLIST * createConnList();
//poll connection client list and handler connect
void pollConnList(CLIENTLIST *cls, HANDLER_CONN handler_conn);
//put a client into list
bool putIntoConnList(CLIENTLIST *cls, CONNCLIENT connclient);
//delete a client from list
bool deleteOneFromConnList(CLIENTLIST *cls, CONNCLIENT connclient);

