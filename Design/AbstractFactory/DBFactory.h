#pragma once
#include "AbstractFactory.h"
class DBFactory : public AbstractFactory
{
public:
	DBFactory(void);
	~DBFactory(void);
	virtual ServerOs * getServerOs(std::string);
	virtual DB * getDBType(std::string);
};

