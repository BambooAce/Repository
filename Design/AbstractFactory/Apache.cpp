#include "Apache.h"


Apache::Apache(void)
{
}


Apache::~Apache(void)
{
}

void deal_request(std::string request)
{
	std::cout<<"Apache recv: " << request<< std::endl; 

}