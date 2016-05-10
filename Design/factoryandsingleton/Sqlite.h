#pragma once
#include <string>
#include <iostream>
#include "DB.h"
class Sqlite: public DB
{
public:
	Sqlite(void);
	Sqlite(std::string s);
	~Sqlite(void);
	virtual void execSQL();
private:
	std::string sql;
};

