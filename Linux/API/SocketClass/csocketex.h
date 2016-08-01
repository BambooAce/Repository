#ifndef CSOCKETEX_H
#define CSOCKETEX_H
class CsocketEX
{
public:
    CsocketEX();
    virtual ~CsocketEX();
    virtual void _SocketEX();
    virtual void _BindEX();
    virtual void _ListenEX();
    virtual void _AcceptEX();
    virtual void _ConnectEX();
    virtual void _SockoptEX();
    void UnknowEX();
};

class SocketEX:public CsocketEX
{
public:
    SocketEX(){}
    ~SocketEX(){}
    virtual void _SocketEX();
};
class BindEX:public CsocketEX
{
public:
    BindEX(){}
    ~BindEX(){}
    virtual void _BindEX();
};
class ListenEX:public CsocketEX
{
public:
    ListenEX(){}
    ~ListenEX(){}
    virtual void _ListenEX();
};
class AcceptEX:public CsocketEX
{
public:
    AcceptEX(){}
    ~AcceptEX(){}
    virtual void _AcceptEX();
};
class ConnectEX:public CsocketEX
{
public:
    ConnectEX(){}
    ~ConnectEX(){}
    virtual void _ConnectEX();
};
class SockoptEX:public CsocketEX
{
public:
    SockoptEX(){}
    ~SockoptEX(){}
    virtual void _SockoptEX();
};

#endif // CSOCKETEX_H
