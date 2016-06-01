#pragma once
#include "AbstractFactory.h"
class ServerOsFactory : public AbstractFactory
{
public:
	ServerOsFactory(void);
	~ServerOsFactory(void);
	virtual ServerOs * getServerOs(std::string);
	virtual DB * getDBType(std::string);
};

