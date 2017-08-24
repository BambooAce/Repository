#include "connlist.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
CLIENTLIST *createConnList()
{
    CLIENTLIST * cls =(CLIENTLIST *)malloc(sizeof(CLIENTLIST));
    if(cls){
        bzero(cls, sizeof(CLIENTLIST));
        cls->header = NULL;
        cls->size = 0;
        return cls;
    }
    return NULL;
}

void pollConnList(CLIENTLIST *cls, HANDLER_CONN handler_conn)
{
    assert(cls);
    CLIENTNODE * cn = cls->header;
    CLIENTNODE * cnn = cn;
    if(!handler_conn)
    {
        return;
    }
    while(cn)
    {
        cnn = cn;
        handler_conn(cn->nclient.cinfo);
        cn = cnn->next;
    }
}

bool putIntoConnList(CLIENTLIST *cls, CONNCLIENT connclient)
{

}

bool deleteOneFromConnList(CLIENTLIST *cls, CONNCLIENT connclient)
{

}
