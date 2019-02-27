#ifndef FINGERMANAGER_H
#define FINGERMANAGER_H
#include "finger.h"

class FingerManager
{
public:
    FingerManager();

    int checkFinger(const char* server_path ,int jobid);

    int job_id;
    int check_result;
    bool chenk_end;
    char m_device_uri[256];
    Finger mFinger;
    const char* server_path;
};

#endif // FINGERMANAGER_H
