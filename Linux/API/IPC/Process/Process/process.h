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
    void InitDeamon();
    Process CreateChildProc();
    std::string GetCommandline();
    pid_t GetPID();
    void WaitChild(pid_t pid = 0);
private:
    pid_t CurPID;
    void ChildRun();
    void ExitRun(Func func);
    Process(Process &proc);
    Process& operator=(Process &proc);
};

#endif // PROCESS_H
