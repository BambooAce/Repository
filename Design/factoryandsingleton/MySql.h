#pragma once
#include "DB.h"
#include <string>
#include <iostream>
class MySql : public DB
{
public:
	MySql();
	MySql(std::string s);
	~MySql();
	virtual void execSQL();
private:
	std::string sql;

};

