#include "mem.h"
#include "iniparser.h"
#include "db.h"
#include "ngserver.h"
#include "deamon.h"
#include "log.h"
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include <locale>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

//configuration file path
#define CONFPATH "./server.conf"

// shared mem pool size 1024*1024*8 bytes.
MEM mem(1024*1024*8);

//this file for log
FILE * fp = NULL;

void * server_run(void *arg);
bool LOGINIT(std::string logpath);

void master(pthread_t sqlthread, pthread_t infoserver_t);

int main()
{
    //    init_deamon();

    //set locale utf8
    std::locale loc("zh_CN.UTF-8");
    std::locale::global(loc);

    //parse conf file
    INIParser ini(CONFPATH);
    DBINFO db= ini.getDBInfo();
    SINFO sin = ini.getSerInfo();

    if(!LOGINIT(sin.logpath))
    {
        fprintf(stderr, "Open log %s fail \n", sin.logpath.c_str());
        _exit(1);
    }
    Log(INFO, "Init mem pool, ngserver will start");
    // init mem pool
    mem.init();

    //set server thread. stack size about 20M.
    pthread_t server_t;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 20971520);
    // begin server thread
    if(0 != pthread_create(&server_t, &attr, server_run, &sin))
    {
        fprintf(stderr, "Create DB thread failed.\n");
        Log(ERROR, "Ngserver pthread start failed");
        pthread_attr_destroy(&attr);
        exit(1);
    }
    Log(INFO, "Ngserver pthread start success");
    // begin db main thread.
    DB myl(db);
    if(myl.connectMysql())
        myl.insertValMyql(&mem);
    else{
        Log(ERROR, "Mysql connect connect failed");
        pthread_attr_destroy(&attr);
        exit(1);
    }
    pthread_attr_destroy(&attr);
    pthread_join(server_t, NULL);
    if(fp)
        fclose(fp);
    return 0;
}

// server main loop
void * server_run(void *arg)
{
    SINFO * ser = (SINFO *)arg;
    InfoServer infoserver(*ser);
    int lisfd = infoserver.bind_socket();
    if(lisfd)
        infoserver.accept_epoll(lisfd, &mem);
    return 0;
}

/**
 * @brief LOGINIT
 * @param logpath
 * @return
 * Initial log system
 */
bool LOGINIT(std::string logpath)
{
    fp = fopen(logpath.c_str(), "a+");
    if(!fp)
    {
        return false;
    }
    return true;
}
