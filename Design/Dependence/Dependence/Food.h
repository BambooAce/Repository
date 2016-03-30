#pragma once
#include <string>
class Food
{
public:
	Food(std::string _food_name, double _price);
	~Food(void);
	std::string get_food_name();
	double get_food_price();
private:
	std::string food_name;
	double price;
};

