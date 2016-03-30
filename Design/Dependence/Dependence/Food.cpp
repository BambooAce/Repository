#include "Food.h"


Food::Food(std::string _food_name, double _price):food_name(_food_name), price(_price)
{
}


Food::~Food(void)
{
}

std::string Food::get_food_name()
{

	return food_name;
}

double Food::get_food_price()
{

	return price;

}