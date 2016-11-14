#ifndef SELECT_H
#define SELECT_H
#include "netmodel.h"
class Select:public NetModel
{
public:
    Select();
    ~Select();
    virtual void Run(Csocket &server);
private:
    Select(const Select &);
    Select &operator =(const Select &);
};

#endif // SELECT_H
