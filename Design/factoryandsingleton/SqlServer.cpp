#include "SqlServer.h"


SqlServer::SqlServer(void)
{
}

SqlServer::SqlServer(std::string s):sql(s)
{

}

void SqlServer::execSQL()
{

	std::cout<< "SQL server exec: "<<sql <<std::endl;
}

SqlServer::~SqlServer(void)
{
}
