#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <string>
#include <unistd.h>

// check header and whether data complete, if not will restore data and continue recv.
void check_header_complete(char *data, int& len, int &otherDataLen, bool &header, bool &complete);

// parse and check data
bool parserAndCheckStream(const char *data, int len, int &IsGinfo, int &sectionSum);

//decrypt data
bool dencryptData(unsigned char* data, int dataLen);

//convert gbk to utf8
void convert(std::string &words);

#endif // IOSTREAM_H
