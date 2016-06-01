#pragma once
#include <string>
#include "DB.h"
#include "ServerOs.h"
class AbstractFactory
{
public:
	AbstractFactory(void);
	~AbstractFactory(void);
	virtual ServerOs * getServerOs(std::string) = 0;
	virtual DB * getDBType(std::string) = 0;
};

