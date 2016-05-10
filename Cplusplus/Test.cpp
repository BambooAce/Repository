#include "Test.h"
#include <iostream>

std::map<std::string, Test *> Test::test;

Test::Test(std::string a):obj(a)
{
	test[obj] = this;
}


Test::~Test(void)
{
}

int main()
{
	Test a("hehehe");
	Test b("heheh");
	std::cout<< (Test::test).size()<<std::endl;
	INT iNt(21);
	iNt++;
	++iNt;
	int ab;
	std::cin >> ab;
	return 0;
}

