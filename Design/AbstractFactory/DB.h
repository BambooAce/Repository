#pragma once
#include <string>
#include <iostream>
class DB
{
public:
	DB(void);
	~DB(void);
	virtual void execsql(std::string) = 0;
};

