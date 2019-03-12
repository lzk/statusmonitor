#ifndef FILTERTASK_H
#define FILTERTASK_H

#ifdef __cplusplus
extern "C" {
#endif

//typedef struct
//{
//    char printer[512];
//    int jobid;
//}
//    filter_task_struct;

//int filter_task_start(filter_task_struct* filter_data);

int filter_task_start(const  char* printer_name ,const char* printer_uri);
int filter_task_end();

#ifdef __cplusplus
}
#endif

#endif // FILTERTASK_H
