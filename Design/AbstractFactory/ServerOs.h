#pragma once
#include <string>
#include <iostream>
class ServerOs
{
public:
	ServerOs(void);
	~ServerOs(void);
	virtual void deal_request(std::string) = 0;
};

