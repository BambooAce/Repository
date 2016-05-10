#pragma once
#include <vector>
#include <string>
#include "DB.h"
class DBFactory
{
	enum {MAXSIZE=20};
public:
	DB * getA_DB(std::string dbname, std::string execsql);
	static void allDBtype();
	static int DB_size();
	static DBFactory *getDBFactory();
	bool addDBtype(std::string dbtype);
	bool deleteDB(std::string dbtype);
private:
	static DBFactory *instance;
	static std::vector<std::string>DBvector;
	DBFactory(void);
	~DBFactory(void);
};
