#ifndef TOMCAT_H
#define TOMCAT_H

typedef
struct{
    int id;
    int is_finger_enable;
    int is_finger_checked;
}
    Job_history;

#include <QStringList>
typedef struct
{
    int pages;
    int current_page;
    QStringList job_list;
}
    Jobs_struct;

class Tomcat
{
public:
    Tomcat();
    static int save_job_history(Job_history* job);
    static int get_job_history(Jobs_struct* jobs);
};

#endif // TOMCAT_H
