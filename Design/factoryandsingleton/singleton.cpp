/*
 * singleton.cpp
 * 
 * Copyright 2016 hpdm <hpdm@hpdm-machine>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <pthread.h>
#include <iostream>
#include <string>
#include <unistd.h>
using std::string;

class Singleton {
private:
  string Name;
  static Singleton *instance;

  Singleton (const string& name) { Name = name;}
  ~Singleton(){}
public:
  void whoAmI(){std::cout << "Name = " << Name << std::endl;}
  static Singleton *getInstance(string name){
    if (instance == NULL)
        instance = new Singleton(name);
    return instance;
  }
};
Singleton * Singleton::instance = NULL;

void *func1(void *arg)
{
	Singleton *instance1 = Singleton::getInstance("instance1");
	instance1->whoAmI();
}

void *func2(void *arg)
{
	Singleton *instance2 = Singleton::getInstance("instance2");
	instance2->whoAmI();
}
void *func3(void *arg)
{
	Singleton *instance3 = Singleton::getInstance("instance3");
	instance3->whoAmI();
}

int main(void){
  pthread_t pthread1;
  pthread_t pthread2;
  pthread_t pthread3;
  pthread_create(&pthread1, NULL,func1, NULL);
                          
  pthread_create(&pthread2, NULL,func2, NULL);
                          
  pthread_create(&pthread3, NULL, func3, NULL);                                         

sleep(5);


  return 0;
}









