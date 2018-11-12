#ifndef TOMCAT_H
#define TOMCAT_H

#include "fingermanager.h"

class Tomcat
{
public:
    Tomcat();

    int getJobHistory(CALLBACK_getJobHistory callback,void* para);
};

#endif // TOMCAT_H
