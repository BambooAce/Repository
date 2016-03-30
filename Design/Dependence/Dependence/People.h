#pragma once
class Book;
class Food;
class People
{
public:
	People(void);
	~People(void);
	void eat(Food& food);
	void read(Book& book);
};

