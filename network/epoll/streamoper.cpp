/**
 *  Include some stream operator.
 *
 */
#include "streamoper.h"
#include "info.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <QString>
#include <QTextCodec>

/**
 * @brief check_header_complete
 * @param clifd
 * @param data
 * @param len
 * @param header
 * @param complete
 * @return
 */
void check_header_complete(char *data, int& len, int &DataLen, bool &header, bool &complete)
{
    PACKHEADER checkheader;
    memcpy(&checkheader, data, sizeof(PACKHEADER));
    if(strcmp(checkheader.MagicNum, "MINFO") == 0 || strcmp(checkheader.MagicNum, "GINFO") == 0)
        header = checkheader.PackageLen > 0 ? true : false;
    if(header){
        complete = checkheader.PackageLen <= len ? true : false;
        if(!complete)
            DataLen = checkheader.PackageLen;
    }
}

/**
 * @brief parserAndCheckStream
 * @param data :Client data stream
 * @param IsGinfo :To GAME info or MACH info, if it`s GameInfo IsGinfo will be 1, MachInfo is 0
 * @param sectionSum : Get package section sum.
 * @return true: Client Data is lawable, or not.
 */
bool parserAndCheckStream(const char *data, int len, int &IsGinfo, int &sectionSum)
{
    PACKHEADER packheader;
    if(len < sizeof(packheader)){
        fprintf(stderr, "len < sizeof(packheader)\n");
        return false;
    }
    memcpy(&packheader, data, sizeof(packheader));
    fprintf(stderr, "\n%s, %d, %d\n", packheader.MagicNum, packheader.PackageLen, packheader.SectionNum);
    if(len <= 0 || packheader.SectionNum <= 0)
    {
        return false;
    }
    //check length
    if(packheader.PackageLen != len)
    {
        fprintf(stderr, "packheader.Packagelen != packlen\n");
        return false;
    }
    // check header message
    if(strcmp(packheader.MagicNum, "GINFO") == 0)
    {
        IsGinfo = 1;
        sectionSum = packheader.SectionNum;
        return true;
    }
    else if(strcmp(packheader.MagicNum, "MINFO") == 0)
    {
        IsGinfo = 0;
        sectionSum = packheader.SectionNum;
        return true;
    }
    else{
        fprintf(stderr ,"flag is not lawable\n");
        return false;
    }
    return true;
}

/**
 * @brief dencryptData
 * @param data
 * @param dataLen
 * @return success or not
 * decrypt data.
 */
bool dencryptData(unsigned char* data, int dataLen)
{
    if ( NULL == data )
    {
        return false;
    }
    static unsigned char codeTable[] = "9876543210zyxwvutsrqponmlkzihgfedcba";
    unsigned char templhs;
    unsigned char temprhs;
    unsigned char temp;
    for ( int dwIndex = 0; dwIndex < dataLen; ++dwIndex )
    {
        temp = data[dwIndex];
        temp ^= codeTable[((dataLen - dwIndex)%sizeof(codeTable))];
        temp = ~temp;
        templhs = temp << 4;
        temprhs = temp >> 4;
        temp = templhs | temprhs;
        data[dwIndex] = temp;
    }
    return true;
}

/**
 * @brief convert
 * @param words gbk string
 * convert gbk to utf8
 */
void convert(std::string &words)
{
    QTextCodec* pCodec = QTextCodec::codecForName("gbk");
    if(pCodec)
    {
        QString qstr = pCodec->toUnicode(words.c_str(), words.length());
        words = qstr.toStdString();
    }
}
