#ifndef TOMCAT_H
#define TOMCAT_H

typedef
struct{
    int id;
    int is_finger_enable;
    int is_finger_checked;
}
    Job_history;

enum{
    JobHistoryIndex_id,
    JobHistoryIndex_printername,
    JobHistoryIndex_hostname,
    JobHistoryIndex_username,
    JobHistoryIndex_filename,
    JobHistoryIndex_copies,
    JobHistoryIndex_isFingerEnable,
    JobHistoryIndex_isFingerChecked,
    JobHistoryIndex_pages,
    JobHistoryIndex_isJobCompleted,
    JobHistoryIndex_time,
};
#include <QStringList>
typedef struct
{
    unsigned int pages;
    unsigned int current_page;
    QStringList job_list;
}
    Jobs_struct;

class Tomcat
{
public:
    Tomcat();
    static int save_job_history(Job_history* job);
    static int get_job_history(Jobs_struct* jobs);
    static int update_job_history();
};

#endif // TOMCAT_H
