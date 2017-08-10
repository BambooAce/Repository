#ifndef INFOMACRO_H
#define INFOMACRO_H
#include <stdio.h>
//get length of string from data and avoid offside
#define GETLEN(LEN, curpoint, endpoint)  do{ \
    if(curpoint <= (endpoint - 4)){  \
        memcpy(&LEN, curpoint, sizeof(int));    \
        curpoint += 4;  \
    }else{   \
        return false;   \
    }   \
}while(0)

//get string and change curpoint location
#define GETSTRVAL(mINFO, LEN, curpoint, endpoint) do{   \
    if(LEN < 0 || LEN >= 256) return false;   \
    if(curpoint <= (endpoint - LEN)){    \
        std::string temp(curpoint, LEN);    \
        mINFO = temp; \
        curpoint += LEN;    \
    }else{  \
        return false; \
    }   \
}while(0)

//base on len, get string and change curpoint location
#define GETLENMINFO(mINFO, LEN, curpoint, endpoint)   do{ \
    GETLEN(LEN, curpoint, endpoint); \
    GETSTRVAL(mINFO, LEN, curpoint, endpoint);   \
}while(0)

#endif // INFOMACRO_H
