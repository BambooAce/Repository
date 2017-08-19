#ifndef DB_H
#define DB_H
#include <mysql.h>
#include "iniparser.h"
#include "mem.h"
#include "info.h"
class DB
{
public:
    DB(DBINFO & db);
    ~DB();
    bool connectMysql();
    void insertValMyql(MEM *QM);
private:
    MYSQL mysql;
    DBINFO dbinfo;
    Info * info;
    int savedNum;
    bool insertsql(const char *str);
    void saveSqltoFile(const char *str);
    FILE *fp;
};

#endif // DB_H
