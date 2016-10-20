#include "process.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#define NOFILE 10
Process::Process()
{
    CurPID = GetPID();
}

Process::~Process()
{

}

Process Process::InitDeamon()
{
    pid_t pid = fork();
    if(pid == -1)
    {
        fprintf(stderr, "Fork1 error\n");
        exit(-1);
    }
    else if(pid == 0)
    {
        setsid();
        pid_t pid2 = fork();
        if(pid2 == -1)
        {
            fprintf(stderr, "Fork2 error\n");
        }else if(pid2 == 0)
        {
            umask(0);
            chdir("/");
            for(int i = 0; i < NOFILE; ++i)
                close(i);
        }else{
            exit(0);
        }
    }else{
        exit(0);
    }
    return Process();
}

bool Process::CreateChildProcAndRun(std::string &cmdline, std::string &arg)
{
    if(fork() == 0)
    {
        execle("","");
    }else{
        exit(0);
    }
}

std::string Process::GetCommandline()
{

}

pid_t Process::GetPID()
{
    return getpid();
}

void Process::WaitChild(pid_t pid)
{

}

void Process::Run(std::string &cmd)
{

}

void Process::ExitRun(Func func)
{
    atexit(func);
}
