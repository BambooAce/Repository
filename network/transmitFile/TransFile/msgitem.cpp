#include "msgitem.h"
#include <string.h>
#include <stdio.h>
//msglen:50\n
//PUT:filename\n
//MD5:345efj48384\n
//SIZE:123485\n
//
//msglen:50\n
//GET:filename\n
//

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
            char *filenametemp = strchr(msg, ':') + 1;
            filename.append(filenametemp);
            if(strncmp(msg, "MD5:", strlen("MD5")) == 0)
            {
                char *md5valuetemp = strchr(msg, ':') + 1;
                md5value.append(md5valuetemp);
            }
            else if(strncmp(msg, "SIZE:", strlen("SIZE:")) == 0)
            {
                char temp[20] = {0};
                sscanf(msg, "%[a-zA-Z:] %d", temp, &filesize);
            }
        }
        else if(strncmp(msg, "GET:", strlen("GET:")) == 0)
        {
            char * getfilename = strchr(msg, ':') + 1;
            filename.append(getfilename);
        }
    }
}





MsgItem::MsgItem(char *msg)
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
    for(;msgtemp[len] != '\0'; msgtemp = line + 1)
    {
        line = strchr(msgtemp, '\n');

    }
}
