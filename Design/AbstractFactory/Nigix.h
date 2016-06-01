#pragma once
#include "ServerOs.h"
class Nigix : public ServerOs
{
public:
	Nigix(void);
	~Nigix(void);
	virtual void deal_request(std::string);
};

