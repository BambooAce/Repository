#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
class String
{
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
        assert(start < end);
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

    ~String()
    {
        delete p;
    }
    String(String &str)
    {

    }
    String & operator=(String &str)
    {
        return *this;
    }
    String * operator&(String &str)
    {
        return &str;
    }
    void show()
    {
        printf("%s\n",p);
    }

private:
    char *p;
    unsigned int len;
};



int main()
{
    String str();
    String str1("sdfsdfsdf");
    str1.show();
    String str2("abcdefghigklmn",1,12);
    str2.show();
    return 0;
}





























