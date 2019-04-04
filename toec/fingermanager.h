#ifndef FINGERMANAGER_H
#define FINGERMANAGER_H
#include "finger.h"

class FingerManager
{
public:
    FingerManager();

    int checkFinger(const char* server_path ,int jobid ,const char* username ,const char* filename);

    int job_id;
    int check_result;
    bool chenk_end;
    int m_timeout;
    char m_device_uri[256];
    Finger mFinger;
    const char* server_path;
    char username[256];
    char filename[256];
//    const char* username;
//    const char* filename;
};

#endif // FINGERMANAGER_H
