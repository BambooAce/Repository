/**
 *
 *  Parse conf file get server and DB message.
 *
*/
#include "iniparser.h"
#include <iostream>

#define SERVER "SERVER"
#define DB "DB"

#define IPADDR "IPADDR"
#define PORT "PORT"
#define LOGLEVEL    "LOGLEVEL"
#define LOGPATH     "LOGPATH"

#define DBADDR      "DBADDR"
#define DBPORT      "DBPORT"
#define DBNAME      "DBNAME"
#define USER      "USER"
#define PASSWORD  "PASSWORD"
#define RESSTR(p) ((p) ? (std::string(p)) : "")
/**
 * @brief INIParser::INIParser
 * @param iniPath server conf file path.
 * load conf file and parse it.
 */
INIParser::INIParser(const char *iniPath):conf(NULL)
{
    conf = NCONF_new(NULL);
    long eline;
    if(conf)
    {
        int ret = NCONF_load(conf, iniPath, &eline);
        if(ret)
        {
            parser();
        }else{
            NCONF_free(conf);
            std::cerr << "Load conf file failed " << std::endl;
            _Exit(1);
        }
    }
}
/**
 * @brief INIParser::~INIParser
 */
INIParser::~INIParser()
{
    if(conf)
        NCONF_free(conf);
}

/**
 * @brief INIParser::getSerInfo
 * @return SINFO IP and port
 */
SInfo INIParser::getSerInfo()
{
    if(sInfo.port)
        return sInfo;
    return SINFO();
}

/**
 * @brief INIParser::getDBInfo
 * @return DBINFO address user password dbname port.
 */
DBInfo INIParser::getDBInfo()
{
    if(!dbInfo.dbAddr.empty() && !dbInfo.dbName.empty() && !dbInfo.dbuser.empty())
        return dbInfo;
    return DBINFO();
}

/**
 * @brief INIParser::parser
 * parse conf file
 */
void INIParser::parser()
{
    char *p;
    p = NCONF_get_string(conf, SERVER, IPADDR);
    sInfo.ipAddr = RESSTR(p);
    p = NCONF_get_string(conf, SERVER, LOGPATH);
    sInfo.logpath = RESSTR(p);
    p = NCONF_get_string(conf, SERVER, PORT);
    sInfo.port = atoi(p);

    p = NCONF_get_string(conf, DB, DBADDR);
    dbInfo.dbAddr = RESSTR(p);
    p = NCONF_get_string(conf, DB, DBPORT);
    dbInfo.dbPort = atoi(p);
    p = NCONF_get_string(conf, DB, DBNAME);
    dbInfo.dbName = RESSTR(p);
    p = NCONF_get_string(conf, DB, USER);
    dbInfo.dbuser = RESSTR(p);
    p = NCONF_get_string(conf, DB, PASSWORD);
    dbInfo.dbpassword = RESSTR(p);
    NCONF_free(conf);
}
