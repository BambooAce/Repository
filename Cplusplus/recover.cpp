#include <iostream>
#include <string>
// not reload
//const std::string func()
//{
//	return std::string("const func");
//}
//std::string func()
//{
//	return std::string("func");
//}
class Base
{
public:
	Base(){}
	~Base(){}
	
	std::string func() const
	{
		return std::string("const func");
	}
	std::string func() 
	{
		return std::string("func");
	}



};

class Child: public Base
{
public:
	Child(){}
	~Child(){}
	std::string func1() const
	{
		return std::string("child const func");
	}
	const std::string func1() 
	{
		return std::string("child func");
	}
};

int main()
{
	const Base base;
	std::cout << base.func() <<std::endl;
	Base base1;
	
	std::cout << base1.func() <<std::endl;
	Child child;
	std::string a = child.func1();
	std::cout << child.func()<<std::endl;
	return 0;
}
