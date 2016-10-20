#ifndef PROCESS_H
#define PROCESS_H
#include <unistd.h>
#include <string>
typedef void (*Func)(void);

class Process
{
public:
    Process();
    ~Process();
    Process InitDeamon();
    bool CreateChildProcAndRun(std::string &cmdline, std::string &arg);
    std::string GetCommandline();
    pid_t GetPID();
    void WaitChild(pid_t pid = 0);
    void Run(std::string &cmd);
    void ExitRun(Func func);
private:
    pid_t CurPID;
    Process(Process &proc);
    Process& operator=(Process &proc);
};

#endif // PROCESS_H
