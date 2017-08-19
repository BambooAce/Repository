#include "log.h"
#include <time.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>

//
extern FILE * fp;

std::string logstr[] = {"[WARN] ", "[ERROR] ", "[INFO] "};

/**
 * @brief gettime
 * @return get time
 */
static std::string gettime()
{
    char timestr[50] = {0};
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    sprintf(timestr, "%d-%d-%d %d:%d:%d ", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1,
            timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    return std::string(timestr);
}

/**
 * @brief Log
 * @param mode
 * @param fmt
 * write log to fp(global arguement)
 */
void Log(int mode, const char *fmt, ...)
{
    std::string log;
    std::string timestr = gettime();
    switch (mode) {
    case WARN:
        log.append(timestr + logstr[0]);
        break;
    case ERROR:
        log.append(timestr + logstr[1]);
        break;
    case INFO:
        log.append(timestr + logstr[2]);
        break;
    default:
        break;
    }
    int n;
    int size = 512;
    char p[512] = {0};
    va_list ap;
    va_start(ap, fmt);
    n = vsnprintf(p, size, fmt, ap);
    va_end(ap);
    if (n < 0)
    {
        return;
    }
    if (n < size)
    {
        char* ptemp = p;
        std::string datastr(ptemp);
        std::string logstr = log+datastr + "\n";
        if(fp)
        {
            flockfile(fp);
            fwrite(logstr.c_str(), 1, logstr.size(), fp);
            fflush(fp);
            funlockfile(fp);
        }
    }
}
