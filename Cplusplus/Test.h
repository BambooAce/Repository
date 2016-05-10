#pragma once
#include <map>
#include <string>
//
// map container 
//
class Test
{
public:
	Test(std::string a);
	~Test(void);
public:
	static std::map<std::string, Test *> test;
private:
	std::string obj;
};

// Int++ ++Int
class INT
{
public:
	INT(int b):a(b)
	{}
	~INT()
	{
	}
	INT &operator++()
	{
		(this->a)++;
		return *this;
	}
	INT operator++(int i)
	{
		INT temp(this->a);
		++(this->a);
		return temp;
	}
private:
	int a;

};


