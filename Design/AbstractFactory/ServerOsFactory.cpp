#include "ServerOsFactory.h"
#include "Apache.h"
#include "Nigix.h"

ServerOsFactory::ServerOsFactory(void)
{
}


ServerOsFactory::~ServerOsFactory(void)
{
}
ServerOs * getServerOs(std::string serveros)
{
	if(serveros == "Apache")
		return new Apache();
	if(serveros == "Nigix")
		return new Nigix();
	return NULL;
}
DB * getDBType(std::string dbtype)
{
	return NULL;
}