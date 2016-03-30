#include <iostream>
#include "People.h"
#include "Book.h"
#include "Food.h"

People::People(void)
{
}


People::~People(void)
{
}

void People::eat(Food& food)
{
	std::cout<<"I eat "<<food.get_food_name()<<" . It is "<<food.get_food_price()<<" $.\n"<<std::cout;
}
void People::read(Book& book)
{
	std::cout<<"I reading a "<<book.get_book_name()<<" . It is "<<book.get_book_price()<<" $.\n"<<std::cout;
}