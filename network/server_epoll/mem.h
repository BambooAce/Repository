#ifndef MEM_H
#define MEM_H
#include <vector>
//save data table
typedef struct LocMap{
    char * point;
    int len;
    LocMap(char *_point, int _len):point(_point), len(_len)
    {}
    bool operator==(const LocMap &lm)
    {
        return (point == lm.point) && (len == lm.len);
    }
}LOCMAP;

class MEM
{
public:
    MEM(int _size);
    bool init();
    bool pushData(char * data, int len);
    bool popDataMap(char *data, int &len);
    void erase(char *data, int len);
    bool isEmpty();
    ~MEM();
private:
    int size;
    int datanum;
    int freemem;
    std::vector<LocMap> memmap;
    char *beginpoint;
    char *curpoint;
};

#endif // MEM_H

