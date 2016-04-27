#include <iostream>
#include <vector>
class Iter
{
public:
    Iter()
    {

    }
    ~Iter()
    {

    }

private:
    std::vector<Iter &> allChild;
};

int main()
{
    return 0;
}
