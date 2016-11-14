#ifndef NETMODEL_H
#define NETMODEL_H
class Csocket;
class NetModel
{
public:
    NetModel();
    ~NetModel();
    virtual void Run(Csocket &server) = 0;
private:
    NetModel(const NetModel &model);
    NetModel &operator=(const NetModel &model);
};

#endif // NETMODEL_H
