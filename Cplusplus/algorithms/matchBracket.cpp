#include <iostream>
#include <stack>
#include <string.h>
using namespace std;
//{[(<>)]}
int IsFit(const char * const str, int len);
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "./main {}{}{}{}{}{}" << std::endl;
        return 1;
    }
    char *str = argv[1];
    int len = strlen(argv[1]);
    if(IsFit(str, len))
        std::cout << "() is Fit" << std::endl;
    else
        std::cout << "() is not Fit" << std::endl;
    return 0;
}

int IsFit(const char * const str, int len)
{
    if(!str || len <= 0)
        return 0;
    int i = 0;
    std::stack<char> charcontainer;
    for(;i < len; ++i)
    {
        int cr = str[i];
        switch (cr) {
        case '{':
            charcontainer.push(cr);
            break;
        case '[':
            charcontainer.push(cr);
            break;
        case '(':
            charcontainer.push(cr);
            break;
        case '<':
            charcontainer.push(cr);
            break;
        case '}':
            if (charcontainer.top() == '{')
            {
                charcontainer.pop();
            }else{
                return 0;
            }
            break;
        case ']':
            if (charcontainer.top() == '[')
            {
                charcontainer.pop();
            }else{
                return 0;
            }
            break;
        case ')':
            if (charcontainer.top() == '(')
            {
                charcontainer.pop();
            }else{
                return 0;
            }
            break;
        case '>':
            if (charcontainer.top() == '<')
            {
                charcontainer.pop();
            }else{
                return 0;
            }
            break;
        default:
            return 0;
        }
    }
    if(charcontainer.size() == 0)
    {
        return 1;
    }
    return 0;
}
