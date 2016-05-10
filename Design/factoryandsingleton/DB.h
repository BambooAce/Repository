#pragma once
class DB
{
public:
	DB(void);
	
	virtual ~DB(void);
	
	virtual void execSQL() = 0;
};

