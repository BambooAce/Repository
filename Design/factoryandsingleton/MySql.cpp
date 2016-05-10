#include "MySql.h"


MySql::MySql():sql("")
{
}

MySql::MySql(std::string s):sql(s)
{


}

MySql::~MySql()
{

}

void MySql::execSQL()
{
	std::cout<<"MYSQL exec: "<<sql<<std::endl;
}