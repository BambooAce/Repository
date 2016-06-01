#include "Sqlite.h"


Sqlite::Sqlite(void)
{
}


Sqlite::~Sqlite(void)
{
}

void Sqlite::execsql(std::string sql)
{
	std::cout<< "Sqlite exec: " << sql<<std::endl;
}