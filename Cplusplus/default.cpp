/*
 *
 *  operator << and >> ,must be friend or normal function.
 *
 *     Valgrind a very good tool to check memery leak...........
 *
 *
 *
 *
 *
 *
 *
 */
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
class String
{
    /*
     * operator << and >> ,must be friend or normal function.
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
    inline String& operator+ (String &str);
    inline String& operator* (unsigned int num);
    inline bool operator== (String &str);
    inline String& operator= (String &str);
    inline String(String &str);
    ~String()
    {
        if(p)
        {
            std::cout<<"delete"<<std::endl;
            free(p);
            p = NULL;
        }
    }


private:
    char *p;
    unsigned int len;
};
String::String(String &str)
{
    this->len = str.len;
    this->p = (char *)malloc(str.len + 1);
    memcpy(this->p, str.p, str.len);
    (this->p)[str.len] = '\0';
}

std::ostream& operator<< (std::ostream& putout, String &str)
{
    putout << str.p <<" Len: "<< str.len << std::endl;
    return putout;
}

String& String::operator+ (String &str)
{
    if(str.len == 0 || str.p == NULL)
    {
        return *this;
    }
    this->len += str.len;
    char *temp = (char *)malloc(this->len+1);
    memset(temp, 0, this->len + 1);
    memcpy(temp, this->p, strlen(this->p)+1);
    memcpy(&temp[strlen(this->p)], str.p, str.len);
    temp[this->len] = '\0';
    free(this->p);
    this->p = temp;
    temp = NULL;
    return *this;
}

String& String::operator* (unsigned int num)
{
    if ((this->p == NULL) || (this->len == 0) || (0 == num) || (1 == num))
    {
        return *this;
    }
    this->len *= num;
    char *temp = (char *)malloc(this->len + 1);
    memset(temp, 0, this->len + 1);
    for (int i = 1;i<=num; ++i)
    {
        size_t length = (i-1) * strlen(this->p);
        memcpy(&temp[length], this->p, strlen(this->p));
    }
    temp[this->len] = '\0';
    free(this->p);
    this->p = temp;
    temp = NULL;
    return *this;
}

bool String::operator== (String &str)
{
    return ((this->p == str.p) && (this->len == str.len));
}

String& String::operator= (String &str)
{
    this->len = str.len;
    if(this->p)
    {
        free(this->p);

    }
    this->p = (char *)malloc(str.len + 1);
    memcpy(this->p, str.p, str.len);
    (this->p)[str.len] = '\0';
    return *this;
}

//
//
//TEST
//

int main()
{
    String str32("acb");
//    std::cout<<str32;
//    String str("abcdefg",2,5);
//    std::cout<< str;
//    String c(str32);
//    c = str32 + str;
//    std::cout<<c;

     //String c = str32 * 3;
    //std::cout<<str32*10;
    String ss(str32 * 1500);
    std::cout<< ss;
    return 0;
}





























