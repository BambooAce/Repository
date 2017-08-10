#ifndef INIPARSER_H
#define INIPARSER_H
#include <string>
#include <openssl/conf.h>

//server info struct IPaddress and port
typedef struct SINFO{
    int port;
    std::string ipAddr;
    std::string logpath;
    SINFO():port(-1), ipAddr(""), logpath("")
    {}
    SINFO(const SINFO &sinfo){
        ipAddr = sinfo.ipAddr;
        port = sinfo.port;
        logpath = sinfo.logpath;
    }
}SInfo;

//DB info user password dbname port.
typedef struct DBINFO{
    std::string dbAddr;
    int dbPort;
    std::string dbuser;
    std::string dbName;
    std::string dbpassword;
    DBINFO():dbAddr(""),
        dbPort(-1),
        dbuser(""),
        dbName(""),
        dbpassword("")
    {}
    DBINFO(const DBINFO &db){
        dbAddr = db.dbAddr;
        dbpassword = db.dbpassword;
        dbPort = db.dbPort;
        dbuser = db.dbuser;
        dbName = db.dbName;
    }
}DBInfo;

class INIParser
{
public:
    INIParser(const char *iniPath);
    ~INIParser();
    SInfo getSerInfo();
    DBInfo getDBInfo();
private:
    SInfo sInfo;
    DBInfo dbInfo;
    CONF * conf;
    void parser();
};

#endif // INIPARSER_H
