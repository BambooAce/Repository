/*
 *
 *  operator << and >> ,must be friend or nomal function.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
class String
{
    /*
     * operator << and >> ,must be friend or nomal function.
     *
     */
    friend inline std::ostream& operator<< (std::ostream& out, String &str);
public:
    String():p(NULL),len(0)
    {

    }
    String(char *q)
    {
        if (q)
        {
            p = (char *)malloc(strlen(q)+1);
            if (p)
            {
                memcpy(p, q, strlen(q));
                p[strlen(q)] = '\0';
                len = strlen(p);
            }
        }
        else
        {
            p = NULL;
            len = 0;
        }
    }
    String(char *q, unsigned int start, unsigned int end)
    {
        if (q)
        {
            p = (char *)malloc(end - start + 1);
            if (p)
            {
                unsigned int i = 0;
                for(unsigned int size = start; size < end; size++,i++)
                {
                    p[i] = q[size];
                }
                p[end - start] = '\0';
                len = end - start;
            }
        }else{
            p = NULL;
            len = 0;
        }
    }
    String& operator +(String &str);
    String& operator *(unsigned int num);
    ~String()
    {
        delete p;
    }


private:
    char *p;
    unsigned int len;
};

std::ostream& operator<< (std::ostream& putout, String &str)
{
    putout << str.p << std::endl;
    return putout;
}

String& String::operator +(String &str)
{
    if (str.len)
        strncat(this->p,str.p, strlen(str.p));
    return *this;
}

String& String::operator *(unsigned int num)
{
    if (num)
    {
        for (int i = 0; i < num; i++)
        {
            char *temp = (char *)malloc(strlen(this->p));
            temp[strlen(this->p)] = '\0';
            strncat(this->p, temp, );
        }
    }
    return *this;
}

int main()
{
    String str32("acecaecesd");
    std::cout<<str32;
    return 0;
}





























