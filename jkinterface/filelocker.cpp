#include "filelocker.h"

#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/file.h>

FileLocker::FileLocker():
    fp(NULL)
{
}

int FileLocker::lock(const char* filename)
{
    int ret = -1;
    fp = fopen(filename, "ab+");
    int fd;
    if(fp){
#ifdef JK_OS_MAC
        fd = fp->_file;
#else
        fd = fp->_fileno;
#endif
        ret = flock(fd, LOCK_EX);
        if (ret){
            fclose(fp);
            fp = NULL;
        }
    }
    return ret;
}

int FileLocker::trylock(const char* filename)
{
    int ret = -1;
    fp = fopen(filename, "ab+");
    int fd;
    if(fp){
#ifdef JK_OS_MAC
        fd = fp->_file;
#else
        fd = fp->_fileno;
#endif
        if (flock(fd, LOCK_EX | LOCK_NB)){
            ret = 0;
        }else{
            fclose(fp);
            fp = NULL;
        }
    }
    return ret;
}

int FileLocker::unlock()
{
    if(fp){
        int fd;
#ifdef JK_OS_MAC
        fd = fp->_file;
#else
        fd = fp->_fileno;
#endif
        flock(fd, LOCK_UN);
        fclose(fp);
        fp = NULL;
    }
    return 0;
}
