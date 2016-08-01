#include "csocketex.h"
#include <stdio.h>
#include <stdlib.h>
CsocketEX::CsocketEX()
{
}

CsocketEX::~CsocketEX()
{
}

void CsocketEX::_SocketEX()
{
    return;
}

void CsocketEX::_BindEX()
{
    return;
}

void CsocketEX::_ListenEX()
{
    return;
}

void CsocketEX::_AcceptEX()
{
}

void CsocketEX::_ConnectEX()
{
    return;
}

void CsocketEX::_SockoptEX()
{
    return;
}

void CsocketEX::UnknowEX()
{
    fprintf(stderr, "Unknow exception\n");
}


void SocketEX::_SocketEX()
{
    CsocketEX::_SocketEX();
    fprintf(stderr, "Build Socket error\n");
}


void BindEX::_BindEX()
{
    CsocketEX::_BindEX();
    fprintf(stderr, "Bind socket error\n");
}


void ListenEX::_ListenEX()
{
    CsocketEX::_ListenEX();
    fprintf(stderr, "Listen Socket error\n");
}


void AcceptEX::_AcceptEX()
{
    CsocketEX::_AcceptEX();
    fprintf(stderr, "Accept Socket error\n");
}


void ConnectEX::_ConnectEX()
{
    CsocketEX::_ConnectEX();
    fprintf(stderr, "Connect Socket error\n");
}

void SockoptEX::_SockoptEX()
{
    CsocketEX::_SockoptEX();
    fprintf(stderr, "Sockopt Socket error\n");
}
