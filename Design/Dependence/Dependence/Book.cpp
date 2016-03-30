#include "Book.h"


Book::Book(std::string _name, double _price):book_name(_name), price(_price)
{
}


Book::~Book(void)
{
}

double Book::get_book_price()
{
	return price;
}

std::string Book::get_book_name()
{

	return book_name;
}