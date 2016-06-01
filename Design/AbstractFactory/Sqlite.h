#pragma once
#include "DB.h"
class Sqlite : public DB
{
public:
	Sqlite(void);
	~Sqlite(void);
	virtual void execsql(std::string sql);
};

