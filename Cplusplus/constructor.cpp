#include <iostream>
class A
{
public:
    A(int b):a(b)
    {
        std::cout<<"constructor"<<std::endl;
    }
    ~A()
    {

    }
    A(A& aa)
    {
        a = aa.a;
        std::cout<<"A(A& aa)"<<std::endl;
    }
    A &operator=(A& aa)
    {
        this->a = aa.a;
        std::cout<<"A &operator=(A& aa)"<<std::endl;
        return *this;
    }

private:
    int a;
};
int main()
{
    //A a = 12; delete A(A& aa) function is ok
    A c(22);
    A d = c;//A(A& aa)
    A e(21);
    e = d; //A &operator=(A& aa)
    return 0;
}
