#ifndef FILTER_CHECK_FINGER_H
#define FILTER_CHECK_FINGER_H

#ifdef __cplusplus
extern "C" {
#endif

enum{
    Checked_Result_OK,
    Checked_Result_Disable,
    Checked_Result_Fail,
    Checked_Result_Cancel,
    Checked_Result_invalidJobid,
    Checked_Result_timeout,
    Checked_Result_checking,
};

int checkFinger(int jobid);

int filter_task_start(const  char* printer_name ,const char* printer_uri);
int filter_task_end();
#ifdef __cplusplus
}
#endif
#endif // FILTER_CHECK_FINGER_H
