#include "msgitem.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
//msglen:50\n
//PUT:filename\n
//MD5:345efj48384\n
//SIZE:123485\n
//
//msglen:50\n
//GET:filename\n

void MsgItem::getValue(char *msg)
{
    if(strlen(msg) > 1)
    {
        if(strncmp(msg, "msglen:", strlen("msglen:")) == 0)
        {
            char temp[20] = {0};
            sscanf(msg, "%[a-zA-Z:] %d", temp, &msglen);
        }
        else if(strncmp(msg, "PUT:", strlen("PUT:")) == 0)
        {
            upload = true;
            char *filenametemp = strchr(msg, ':') + 1;
            filename.append(filenametemp);

        }
        else if(strncmp(msg, "MD5:", strlen("MD5")) == 0)
        {
            char *md5valuetemp = strchr(msg, ':') + 1;
            md5value.append(md5valuetemp);
        }
        else if(strncmp(msg, "SIZE:", strlen("SIZE:")) == 0)
        {
            char temp[20] = {0};
            sscanf(msg, "%[a-zA-Z:] %d", temp, &filesize);
        }
        else if(strncmp(msg, "GET:", strlen("GET:")) == 0)
        {
            upload = false;
            char * getfilename = strchr(msg, ':') + 1;
            filename.append(getfilename);
        }
    }
}

int MsgItem::getMsglen()
{
    if(msglen > 0)
        return msglen;
    return 0;
}

MsgItem::MsgItem():msglen(0), filesize(0), md5value(""), filename(""), upload(false)
{
}

int MsgItem::getFileSize() const
{
    return filesize;
}

bool MsgItem::setFileSize(int size)
{
    if(size)
    {
        filesize = size;
        return true;
    }
    return false;
}

std::string MsgItem::getMD5Value() const
{
    return md5value;
}

bool MsgItem::setMD5Value(std::string md)
{
    if(md.length() > 0)
    {
        md5value = md;
        return true;
    }
    return false;
}

std::string MsgItem::getFilename() const
{
    return filename;
}

void MsgItem::parseline(char *msg)
{
    char *msgtemp = msg;
    char *line = NULL;
    int len = 0;
    for(;msg[len] != '\0'; msgtemp = line + 1)
    {
        char temp[256] = {0};
        line = strchr(msgtemp, '\n');
        memcpy(temp, msgtemp, line - msgtemp + 1);
        temp[line-msgtemp] = '\0';
        getValue(temp);
        len = (int)(line - msg) + 1;
    }
}

bool MsgItem::isUpload() const
{
    return upload;
}

std::string MsgItem::createPutMsg(int msg_len, int file_size, std::string md5_value, std::string file_name)
{
    std::string msg;
    msglen = msg_len;
    filesize = file_size;
    md5value = md5_value;
    filename = file_name;
    msg << "msglen:" <<msglen<<"\n"<<"PUT:"<<filename<<"\n"<<"MD5:"<<md5value<<"\n"<<"SIZE:"<<filesize<<"\n";
    return msg;
}

std::string MsgItem::createGetMsg(int msg_len, std::string file_name)
{
    std::string msg;
    msglen = msg_len;
    filename = file_name;
    msg<< "msglen:"<<msglen <<"\n"<<"GET:"<<filename<<"\n";
    return msg;
}

int main()
{
  //  char msg[] ="msglen:50\nPUT:filename\nMD5:345efj48384\nSIZE:123485\n";
    //char msg[] ="msglen:50\nGET:filename\nMD5:dfsdfsdfdfsdf\nSIZE:234234\n";
    MsgItem mi;
    std::string test = mi.createGetMsg(32, "update.ini");
    test.
    //mi.parseline(test.c_str());
    std::cout<<"FileName: "<<mi.getFilename() << "\nFileSize: "<<mi.getFileSize()<<"\nMD5Value: "<< mi.getMD5Value() << "\nMsglen: "<<mi.getMsglen() <<std::endl;
    return 0;
}
