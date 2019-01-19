#ifndef FILELOCKER_H
#define FILELOCKER_H

#include <stdio.h>

class FileLocker
{
public:
    FileLocker();

protected:
    int lock(const char* path);
    int trylock(const char* path);
    int unlock();
    FILE* fp;
};

#endif // FILELOCKER_H
