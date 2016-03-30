#include "People.h"
#include "Book.h"
#include "Food.h"
#include <stdio.h>
int main()
{

	People people;
	Book book("ACE network process", 45.32);
	Food food("Nodle", 7);
	people.eat(food);
	people.read(book);
	getchar();
	return 0;
}