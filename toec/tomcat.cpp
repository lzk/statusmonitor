#include "tomcat.h"
#include "jkinterface.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <QSettings>
static const char* job_history_file_name = "/tmp/job_history.xml";
static const char* job_key ="jobs/id/";
const int jobs_per_page = 20;
Tomcat::Tomcat()
{

}

static void callback_getJob(void* para,Job_struct* js)
{
    Job_history* job = (Job_history*)para;
    if(js->id != job->id)
        return;

    int printer_result = 1;

    char hostname[256];
    gethostname(hostname ,sizeof(hostname));
//    StatusMonitor* sm = (StatusMonitor*)para;
    char job_history[256];
    sprintf(job_history ,"%d,%s,%s,%s,%s,%d,%d,%ld,%d,%d,%d"
            ,js->id ,js->printer ,hostname,js->user_name  ,js->name
            ,js->pages_completed ,js->copies ,js->timet
            ,job->is_finger_enable,job->is_finger_checked,printer_result//是，成功，否
            );
//    sprintf(job_history ,"echo %s >> %s" ,job_history ,job_history_file_name);
//    system(job_history);

    QSettings settings(job_history_file_name ,QSettings::defaultFormat());
    QString key = QString().sprintf("%s%d" ,job_key ,job->id);
    settings.setValue(key ,QString(job_history));
}

int Tomcat::save_job_history(Job_history* job)
{
    cups_get_job(callback_getJob ,(void*)job);
}

int Tomcat::get_job_history(Jobs_struct* pJobs)
{
    QSettings settings(job_history_file_name ,QSettings::defaultFormat());
    QString key(job_key);
    settings.beginGroup(key);
    QStringList jobs = settings.childKeys();

    int num_of_jobs = jobs.count();
    pJobs->pages = num_of_jobs / jobs_per_page + 1;
    if(pJobs->current_page > pJobs->pages)
        pJobs->current_page = pJobs->pages - 1;
    int index = pJobs->current_page * jobs_per_page;
    pJobs->job_list.clear();

    for(int i = 0 ;i < jobs_per_page ;i++ ,index++){
        if(index > num_of_jobs - 1)
            break;
        pJobs->job_list << settings.value(jobs.at(index)).toString();
    }
    settings.endGroup();

}
