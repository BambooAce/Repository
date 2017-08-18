typedef void (*ExecuteTask)(void **arg);

typedef struct taskpackage{
    void * arg;
    ExecuteTask et;
    int status;
}TASKPACKAGE;

typedef struct{
    int priority;
    TASKPACKAGE *tg;
    TASKPACKAGE *pretask;
    TASKPACKAGE *next;
}TaskNode;

typedef struct {
    TaskNode * header;
    unsigned int num;
}TASKLIST;

int initTaskList(TASKLIST *tl);
TASKPACKAGE * getATask(TASKLIST *tl);
int putATask(TASKLIST *tl, ExecuteTask runtask, void *arg, unsigned int argsize, int priority);
int deleteTask(TASKLIST *tl, TASKPACKAGE tg);
int clearTaskList(TASKLIST *tl);
int getTaskNum(TASKLIST * tl);
