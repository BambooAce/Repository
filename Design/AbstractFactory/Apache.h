#pragma once
#include "ServerOs.h"
class Apache : public ServerOs
{
public:
	Apache(void);
	~Apache(void);
	virtual void deal_request(std::string);
};

