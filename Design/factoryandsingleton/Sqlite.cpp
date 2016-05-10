#include "Sqlite.h"


Sqlite::Sqlite(void)
{
}


Sqlite::~Sqlite(void)
{
}

Sqlite::Sqlite(std::string s):sql(s)
{


}

void Sqlite::execSQL()
{
	std::cout << "Sqlite exec: "<< sql << std::endl;
}