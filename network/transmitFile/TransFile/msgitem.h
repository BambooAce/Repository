#ifndef MSGITEM_H
#define MSGITEM_H
#include <string>
class MsgItem
{
public:
    MsgItem(char *msg);
    void getValue(char *msg);
    void setMsglen();
    int getMsglen();
    int getFileSize() const;
    bool setFileSize(int size);
    std::string getMD5Value() const;
    bool setMD5Value(std::string md);
    std::string getFilename() const;
    void parseline(char *msg);
private:
    int msglen;
    int filesize;
    std::string md5value;
    std::string filename;

};

#endif // MSGITEM_H
