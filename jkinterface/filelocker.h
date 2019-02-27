#ifndef FILELOCKER_H
#define FILELOCKER_H

#include <stdio.h>

class FileLocker
{
public:
    FileLocker();

protected:
    int lock(const char* path);
    int unlock();
    FILE* fp;
    char lock_file[256];
};

#endif // FILELOCKER_H
