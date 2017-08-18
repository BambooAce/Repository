#include "task.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
int initTaskList(TASKLIST *tl)
{
    tl->header = NULL;
    tl->num = 0;
    return 1;
}

TASKPACKAGE *getATask(TASKLIST *tl)
{
    assert(tl);
    if(tl->num)
    {
        int i;
        for(i = 0; i < tl->num; ++i)
        {
            tl->header->
        }
    }
    return NULL;
}

int putATask(TASKLIST *tl, ExecuteTask runtask, void *arg, unsigned int argsize, int priority)
{
    assert(tl);
    TASKPACKAGE * tp = (TASKPACKAGE *)malloc(sizeof(TASKPACKAGE));
    if(tp && arg){
        memcpy(tp->arg, arg, argsize);
    }else
        tp->arg = NULL;
    tp->et = runtask;
    TaskNode *atask = (TaskNode *)malloc(sizeof(TaskNode));
    if(atask){
        atask->tg = tp;
        atask->priority = priority > 0 ? priority : 1;
        if(tl->header){
            tl->header->pretask = atask;
            atask->next = tl->header;
            atask->pretask = tl;
        }else{
            tl->header = atask;
            atask->pretask = NULL;
            atask->next = NULL;
        }
        tl->num++;
        return 1;
    }
    return 0;
}


int clearTaskList(TASKLIST *tl)
{
    if(tl->num)
    {
        TaskNode * tn = tl->header;
        while(tn)
        {
            if(tn->tg->arg){
                free(tn->tg->arg);
                tn->tg->arg = NULL;
            }
            if(tn->tg)
            {
                free(tn->tg);
                tn->tg = NULL;
            }
            free(tn);
            tn = NULL;
            tn = tn->next;
            tl->num--;
        }
    }
    return 1;
}

int deleteTask(TASKLIST *tl, TASKPACKAGE tg)
{
    assert(tl);
    if(tl->num)
    {
        TaskNode *tn = tl->header;
        while(tn)
        {
            if(tn->tg){
                memcmp()
            }
        }
    }
    return 1;
}

int getTaskNum(TASKLIST *tl)
{
    return tl->num;
}
