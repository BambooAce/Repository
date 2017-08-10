#ifndef LOG_H
#define LOG_H
#include <stdio.h>
enum LOG{
    WARN,
    ERROR,
    INFO
};
void Log(int mode, const char *fmt, ...);
#endif // LOG_H
