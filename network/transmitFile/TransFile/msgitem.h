#ifndef MSGITEM_H
#define MSGITEM_H
#include <string>
class MsgItem
{
public:
    MsgItem();
    void getValue(char *msg);
    void setMsglen();
    int getMsglen();
    int getFileSize() const;
    bool setFileSize(int size);
    std::string getMD5Value() const;
    bool setMD5Value(std::string md);
    std::string getFilename() const;
    void parseline(char *msg);
    bool isUpload() const;
    std::string createPutMsg(int msg_len, int file_size, std::string md5_value, std::string file_name);
    std::string createGetMsg(int msg_len, std::string file_name);
private:
    int msglen;
    int filesize;
    std::string md5value;
    std::string filename;
    bool upload;

};

#endif // MSGITEM_H
