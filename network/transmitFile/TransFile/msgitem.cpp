#include "msgitem.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

//PUT:filename\n
//MD5:345efj48384\n
//SIZE:123485\n
//

//GET:filename\n

void MsgItem::getValue(char *msg)
{
    if(strlen(msg) > 1)
    {
        if(strncmp(msg, "PUT:", strlen("PUT:")) == 0)
        {
            upload = true;
            char *filenametemp = strchr(msg, ':') + 1;
            filename = "";
            filename.append(filenametemp);
        }
        else if(strncmp(msg, "MD5:", strlen("MD5")) == 0)
        {
            char *md5valuetemp = strchr(msg, ':') + 1;
            md5value = "";
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
            filename = "";
            filename.append(getfilename);
        }
    }
}

MsgItem::MsgItem():filesize(0), md5value(""), filename(""), upload(false)
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

void MsgItem::parseline(const char *msg)
{
    if(!msg)
        return;
    char *msgtemp= (char *)malloc(strlen(msg)+1);
    char *freemsg = msgtemp;
    bzero(msgtemp, strlen(msg) + 1);
    memccpy(msgtemp, msg, 0, strlen(msg));
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
    free(freemsg);
    freemsg = NULL;
}

bool MsgItem::isUpload() const
{
    return upload;
}

std::string MsgItem::createPutMsg(int file_size, std::string md5_value, std::string file_name)
{
    char msg[256] = {0};
    filesize = file_size;
    md5value = md5_value;
    filename = file_name;
    sprintf(msg,"PUT:%s\nMD5:%s\nSIZE:%d\n", file_name.c_str(), md5_value.c_str(), file_size);
    return std::string(msg);
}

std::string MsgItem::createGetMsg(std::string file_name)
{
    char msg[256] = {0};
    filename = file_name;
    sprintf(msg,"GET:%s\n", file_name.c_str());
    return msg;
}
