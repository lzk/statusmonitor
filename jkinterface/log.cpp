#include "log.h"
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
const char* log_file = "/tmp/loglog.log";
const char* app_version = "1.0.0.3";
class JKLog
{
public:
  JKLog();
  ~JKLog();
  int log(const char*  ,va_list pArgs);

private:
  FILE* file;
};

JKLog::JKLog()
{
    struct timeb tp;
    ftime(&tp);
    time_t t;
    struct tm *p_time;
    time(&t);
    p_time = gmtime(&t);

    char tmp_filename[1024];
    sprintf(tmp_filename ,"/tmp/loglog_%04d%02d%02d%02d%02d%02d.log"
            ,p_time->tm_year + 1900 ,p_time->tm_mon + 1 ,p_time->tm_mday
            ,p_time->tm_hour ,p_time->tm_min ,p_time->tm_sec);

    file = fopen(tmp_filename ,"w+");
    fprintf(file ,"--------VOP v%s-------\n\n" ,app_version);
    fflush(file);
    fclose(file);

    int ret;
    char cmd[1024];
    sprintf(cmd ,"chmod 666 %s 2>> %s" ,tmp_filename ,tmp_filename);
    ret = system(cmd);
    sprintf(cmd ,"rm %s 2>> %s" ,log_file ,tmp_filename);
    ret = system(cmd);
    sprintf(cmd ,"ln -s %s %s 2>> %s" ,tmp_filename ,log_file ,tmp_filename);
    ret = system(cmd);
    sprintf(cmd ,"uname -a >> %s" ,tmp_filename);
    ret = system(cmd);
    sprintf(cmd ,"cat /etc/issue >> %s" ,tmp_filename);
    ret = system(cmd);

    file = fopen(tmp_filename ,"a");
//    file = fopen(log_file ,"a");

//    file = fopen(log_file ,"w+");
}

JKLog::~JKLog()
{
    fclose(file);
}

int JKLog::log(const char* para ,va_list pArgs)
{
    int ret = vfprintf(file ,para ,pArgs);
    fflush(file);
    return ret;
}

static JKLog gJKLog;
int jklog(const char* para ,...)
{
    int result;
    va_list pArgs;
    va_start(pArgs, para);
    result = gJKLog.log(para ,pArgs);
    va_end(pArgs);
    return result;
}
