/**
 *
 * check data, parse data and restore to mysql.
 *
*/
#include "db.h"
#include "machinfo.h"
#include "gameinfo.h"
#include "streamoper.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale>
#include <string.h>
#include <unistd.h>
#include <string>
#include <iostream>

#define FILETEMP "./sqldata.temp"
#define FILETEMP1 "./sqldata.temp.bak"
char datastr[1024] = {0};

/**
 * @brief DB::DB
 * @param db
 */
DB::DB(DBINFO &db):dbinfo(db),info(0), fp(0), savedNum(0)
{
}

DB::~DB()
{
    mysql_close(&mysql);
    if(fp)
        fclose(fp);
}

/**
 * @brief DB::connectMysql
 * @return connect success or not.
 */
bool DB::connectMysql()
{
    mysql_init(&mysql);
    char value = 1;
    mysql_options(&mysql, MYSQL_OPT_RECONNECT, &value);
    if(!mysql_real_connect(&mysql, dbinfo.dbAddr.c_str(), dbinfo.dbuser.c_str(),
                           dbinfo.dbpassword.c_str(), dbinfo.dbName.c_str(),
                           dbinfo.dbPort, 0, CLIENT_MULTI_STATEMENTS))
    {
        fprintf(stderr, "Error connecting to database: %s\n", mysql_error(&mysql));
        Log(ERROR,  "Error connecting to database: %s\n", mysql_error(&mysql));
        return false;
    }
    Log(INFO, "Connect Mysql success");
    return true;
}

/**
 * @brief DB::insertValMyql
 * @param QM  shared MEM
 * If QM is not empty, pop data from QM and parse lawable, then put them into GAMEINFO or MACHINFO
 */
void DB::insertValMyql(MEM *QM)
{
   if(!QM)
       return;
   while(1){
       if(!QM->isEmpty())
       {
           int len = 0;
           QM->popDataMap(datastr, len);
           if(len)
           {
               int isGinfo = -1;
               int sectionSum = 0;
               // check data if lawable or not, isGinfo is GAMEINFO or MACHINFO
               if(!parserAndCheckStream(datastr, len, isGinfo, sectionSum))
               {
                   fprintf(stderr, "data is not lawable\n");
                   Log(ERROR, "Recved Data is not lawable");
                   continue;
               }
               // set locale to use Chinese.
               std::locale loc("zh_CN.UTF-8");
               std::locale::global(loc);
               if(isGinfo == 1){
                    info = new GameInfo;
               }
               else if(isGinfo == 0){
                    info = new MachInfo;
               }
               if(info)
               {
                   fprintf(stderr, "before getinfo\n");
                   if(info->getInfo(datastr, len, sectionSum))
                   {
                       std::string sql = info->infoToStr();
                       bool insert = insertsql(sql.c_str());
                       if(!insert)
                       {
                           saveSqltoFile(sql.c_str());
                       }
                   }
                   delete info;
               }
           }
           bzero(datastr, 1024);
       }else{
           usleep(1000);
       }
   }
}

/**
 * @brief DB::insertsql
 * @param str sql after parsed.
 * @return if sql query success will return true, or not.
 */
bool DB::insertsql(const char *str)
{
    int res = 0;
    res = mysql_query(&mysql, "SET NAMES UTF8");
    if(res != 0)
    {
        return false;
    }
    res = mysql_query(&mysql, str);
    if(!res)
    {
         fprintf(stderr, "Inserted %lu rows\n", (unsigned long)mysql_affected_rows(&mysql));
         return true;
    }else{
         fprintf(stderr, "Insert error %d: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
         Log(ERROR, "Insert error %d: %s\n", mysql_errno(&mysql), mysql_error(&mysql));
         return false;
    }
}

/**
 * @brief DB::saveSqltoFile
 * @param str
 * If sql insert error, it will record sql into FILETEMP or FILETEMP in exchange.
 * Will have other script file to run mysql command to execute those sql file.
 */
void DB::saveSqltoFile(const char *str)
{
    if(!fp){
        fp = fopen(FILETEMP, "w");
    }
    if(savedNum >= 600)
    {
        fclose(fp);
        fp = fopen(FILETEMP1, "w");
    }
    char temp[1024] = {0};
    memcpy(temp, str, strlen(str));
    if(fp)
    {
        fwrite(str, strlen(str), 1, fp);
        fflush(fp);
        savedNum++;
    }
}
