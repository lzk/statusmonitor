#include "log.h"
#include <stdarg.h>
#include <stdlib.h>
const char* log_file = "/tmp/loglog.log";

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
//    char tmp_filename[L_tmpnam];
//    char* tmp_name = tmpnam(tmp_filename);
//    sprintf(tmp_filename ,"/tmp/loglog_%s.log" ,tmp_name);
//    file = fopen(tmp_filename ,"w+");
//    fprintf(file ,"--------log-------\n\n");
//    fclose(file);

//    char cmd[1024];
//    sprintf(cmd ,"chmod 666 %s 2>> %s" ,tmp_filename ,tmp_filename);
//    system(cmd);
//    sprintf(cmd ,"rm %s 2>> %s" ,log_file ,tmp_filename);
//    system(cmd);
//    sprintf(cmd ,"ln -s %s %s 2>> %s" ,tmp_filename ,log_file ,tmp_filename);
//    system(cmd);
//    sprintf(cmd ,"uname -a >> %s" ,tmp_filename);
//    system(cmd);
//    sprintf(cmd ,"cat /etc/issue >> %s" ,tmp_filename);
//    system(cmd);

//    file = fopen(tmp_filename ,"w");

    file = fopen(log_file ,"w+");
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
