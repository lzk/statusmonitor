#include "tomcat.h"

Tomcat::Tomcat()
{

}

int Tomcat::getJobHistory(CALLBACK_getJobHistory callback,void* para)
{
    FingerManager fm;
    return fm.getJobHistory(callback ,para);
}
