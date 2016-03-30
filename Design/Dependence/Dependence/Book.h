#pragma once
#include <string>
class Book
{
public:
	Book(std::string _name, double _price = 29.22);
	~Book(void);
	std::string get_book_name();
	double get_book_price();
private:
	std::string book_name;
	double price;
};

