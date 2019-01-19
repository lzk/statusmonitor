#ifndef FINGERMANAGER_H
#define FINGERMANAGER_H

class FingerManager
{
public:
    FingerManager();

    int checkFinger(const char* server_path ,int jobid);

    int job_id;
    int check_result;
    const char* server_path;
};

#endif // FINGERMANAGER_H
