#include "process.h"
#include <stdlib.h>
Process::Process()
{
    CurPID = GetPID();
}

Process::~Process()
{

}

void Process::InitDeamon()
{

}

Process Process::CreateChildProc()
{

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

void Process::ChildRun()
{

}

void Process::ExitRun(Func func)
{
    atexit(func);
}
