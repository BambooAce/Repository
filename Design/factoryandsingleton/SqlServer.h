#pragma once
#include "DB.h"
#include <string>
#include <iostream>
class SqlServer : public DB
{
public:
	SqlServer(void);
	~SqlServer(void);
	SqlServer(std::string s);
	virtual void execSQL();
private:
	std::string sql;
};

