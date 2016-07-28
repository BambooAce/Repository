#ifndef CSOCKETEX_H
#define CSOCKETEX_H
class CsocketEX
{
public:
    CsocketEX();
    ~CsocketEX();
    virtual void SocketEX();
    virtual void BindEX();
    virtual void ListenEX();
    virtual void AcceptEX();
    virtual void ConnectEX();
    virtual void SockoptEX();
    void UnknowEX();
};

class SocketEX:public CsocketEX
{
public:
    virtual void Socket_EX();
};
class BindEX:public CsocketEX
{
public:
    virtual void Bind_EX();
};
class ListenEX:public CsocketEX
{
public:
    virtual void Listen_EX();
};
class AcceptEX:public CsocketEX
{
public:
    virtual void Accept_EX();
};
class ConnectEX:public CsocketEX
{
public:
    virtual void Connect_EX();
};
class SockoptEX:public CsocketEX
{
public:
    virtual void Sockopt_EX();
};

#endif // CSOCKETEX_H
