#ifndef MSGITEM_H
#define MSGITEM_H
#include <string>
class MsgItem
{
public:
    MsgItem();
    void getValue(char *msg);
    int getFileSize() const;
    bool setFileSize(int size);
    std::string getMD5Value() const;
    bool setMD5Value(std::string md);
    std::string getFilename() const;
    void parseline(const char *msg);
    bool isUpload() const;
    std::string createPutMsg(int file_size, std::string md5_value, std::string file_name);
    std::string createGetMsg(std::string file_name);
private:
    int filesize;
    std::string md5value;
    std::string filename;
    bool upload;

};

#endif // MSGITEM_H
