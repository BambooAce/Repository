#include "DBFactory.h"
#include "MySql.h"
#include "Sqlite.h"
#include "SqlServer.h"
DBFactory * DBFactory::instance = NULL;
std::vector<std::string> DBFactory::DBvector(MAXSIZE);
void DBFactory::allDBtype()
{
	DBvector.push_back("MySQL");
	DBvector.push_back("SQLite");
	DBvector.push_back("SQLServer");
}

bool DBFactory::addDBtype(std::string dbtype)
{
	std::vector<std::string>::iterator iter;
	for (iter = DBvector.begin(); iter != DBvector.end(); ++iter)
	{
		if (*iter == dbtype)
		{
			return false;	
		}
	}
	DBvector.push_back(dbtype);
	return true;
}

bool DBFactory::deleteDB(std::string dbtype)
{
	std::vector<std::string>::iterator iter;
	for (iter = DBvector.begin(); iter != DBvector.end(); ++iter)
	{
		if (*iter == dbtype)
		{
			DBvector.erase(iter);
			return true;
		}
	}
	return false;

}

DBFactory::DBFactory(void)
{
}

DBFactory::~DBFactory(void)
{
}


DB * DBFactory::getA_DB(std::string dbname, std::string execsql)
{
	std::vector<std::string>::iterator iter;
	iter = DBvector.begin();
	for (iter;iter != DBvector.end(); ++iter)
	{
		if(*iter == dbname)
		{
			if(dbname == "MySQL")
				return new MySql(execsql);
			if(dbname == "SQLite")
				return new Sqlite(execsql);
			if(dbname == "SQLServer")
				return new SqlServer(execsql);
		}
	}
	return NULL;
}

int DBFactory::DB_size()
{

	return DBvector.size();
}

DBFactory *DBFactory::getDBFactory()
{
	if(!instance)
		instance = new DBFactory();
	allDBtype();
	return instance;
}

int main()
{
	DBFactory * dbfactory = DBFactory::getDBFactory();
	dbfactory->deleteDB("MySQL");
	DB * db = dbfactory->getA_DB("SQLServer", "select * from table1");
	if(db)
		db->execSQL();
	int a;
	std::cin>>a;
	return 0;
}