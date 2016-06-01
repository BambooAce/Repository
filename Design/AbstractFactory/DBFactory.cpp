#include "DBFactory.h"


DBFactory::DBFactory(void)
{
}


DBFactory::~DBFactory(void)
{
}
ServerOs * getServerOs(std::string os)
{
	return NULL;
}
DB * getDBType(std::string dbtype)
{
	if(dbtype == "Mysql")
		return new Mysql();
	if(dbtype == "Sqlite")
		return new Sqlite();
	return NULL;
}