/**
 *
 * |    <---- point;   <point, len1>
 * |
 * |
 * |                   <point+3, len2>
 * |
 * |                   curpoint = point + 3 + len2; datanum = len1 + len2
 * |                   freemem = len - datanum;
 * Simple memory pool , like stack. A table to manage mem pool. Thread safe.
 *
 */

#include "mem.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <pthread.h>
/**
 * @brief MEM::MEM
 * @param _size
 */
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief MEM::MEM
 * @param _size Mem pool size
 */
MEM::MEM(int _size):size(_size), datanum(0), freemem(_size), beginpoint(0), curpoint(0)
{
}

/**
 * @brief MEM::init
 * @return mem pool init success or not
 *
 */
bool MEM::init()
{
    beginpoint = (char *)malloc(size);
    if(beginpoint)
    {
        memset(beginpoint, 0, size);
        curpoint = beginpoint;
        return true;
    }
    return false;
}

/**
 * @brief MEM::pushData
 * @param data
 * @param len
 * @return success or not
 * push data into mem pool and if mem size is not enough, it will resize mem.
 */
bool MEM::pushData(char *data, int len)
{
    pthread_mutex_lock(&mutex);
    if(data && (len >= 0))
    {
        if(len > freemem){
            // resize mem pool
            char * newbegin = (char *)malloc(size*2);
            if(newbegin)
            {
                memset(newbegin, 0, size*2);
                memcpy(newbegin, beginpoint, datanum);
                freemem = size*2 - datanum;
                curpoint = newbegin + datanum;
                std::vector<LOCMAP>::iterator it;
                for(it = memmap.begin(); it != memmap.end(); ++it)
                {
                    it->point = it->point - beginpoint + newbegin;
                }
                free(beginpoint);
                beginpoint = newbegin;
                size *= 2;
            }else{
                fprintf(stderr, "System memory is not enough.\n");
				pthread_mutex_unlock(&mutex);
                return false;
            }
        }
        //put data into mem pool, and modify curporint freesize, update locmap
        if(memmove(curpoint, data, len))
        {
            LOCMAP loc(curpoint, len);
            curpoint += len;
            freemem -= len;
            datanum += len;
            memmap.push_back(loc);
			pthread_mutex_unlock(&mutex);
            return true;
        }
    }
    pthread_mutex_unlock(&mutex);
    return false;
}

/**
 * @brief MEM::erase
 * @param data data point
 * @param len length of data
 * erase data from data point and length len.
 */
void MEM::erase(char *data, int len)
{
    LOCMAP temp(data, len);
    std::vector<LOCMAP>::iterator it = std::find(memmap.begin(), memmap.end(), temp);
    if(it != memmap.end())
    {
        bzero(it->point, it->len);
        it->point = NULL;
        datanum -= it->len;
		freemem += it->len;
		curpoint -= it->len;
        it = memmap.erase(it);
    }
}

/**
 * @brief MEM::popDataMap
 * @param data
 * @param len
 * pop into data from mem pool and erase data.
 */
bool MEM::popDataMap(char *data, int &len)
{
    pthread_mutex_lock(&mutex);
    if(datanum)
    {
        if(memmap.size()){
            len = memmap.back().len;
            if(len){
                memcpy(data, memmap.back().point, len);
            	erase(memmap.back().point, len);
            }
            else{
                len = 0;
            	pthread_mutex_unlock(&mutex);
				return false;
            }
            pthread_mutex_unlock(&mutex);
            return true;
        }
    }
    pthread_mutex_unlock(&mutex);
    return false;
}

/**
 * @brief MEM::isEmpty
 * @return empty or not
 */
bool MEM::isEmpty()
{
    pthread_mutex_lock(&mutex);
    if(datanum)
    {
        pthread_mutex_unlock(&mutex);
        return false;
    }
    pthread_mutex_unlock(&mutex);
    return true;
}

MEM::~MEM()
{
    if(beginpoint){
        free(beginpoint);
        beginpoint = NULL;
        curpoint = NULL;
    }
}
