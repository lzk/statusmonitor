#ifndef FINGERMANAGER_H
#define FINGERMANAGER_H

typedef void (*CALLBACK_getJobHistory)(void* ,const char*);

class FingerManager
{
public:
    FingerManager();

    int checkFinger(const char* server_path ,int jobid);
    int getJobHistory(CALLBACK_getJobHistory callback,void* para);

    int job_id;
    int isFingerEnable;
    int isFingerChecked;
    int printerResult;
};

#endif // FINGERMANAGER_H
