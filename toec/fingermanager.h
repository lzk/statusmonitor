#ifndef FINGERMANAGER_H
#define FINGERMANAGER_H
#include "finger.h"

class FingerManager
{
public:
    FingerManager();

    int checkFinger(const char* server_path ,int jobid ,const char* username ,const char* filename ,int use_finger_checking);

    int job_id;
    int check_result;
    bool chenk_end;

    int m_timeout;
    char m_device_uri[256];
    Finger mFinger;
    const char* server_path;
    char username[256];
    char filename[256];
    int use_finger_checking;
//    static bool print_cancel;
//    static int job_id_cancel;
    //static void abort_print(int p_job_id, bool p_print_cancel);

//    const char* username;
//    const char* filename;
};

#endif // FINGERMANAGER_H
