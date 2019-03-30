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
    Checked_Result_NoFinger,
    Checked_Result_DevBusy,
};

int checkFinger(int jobid ,const char* username ,const char* filename);

int get_device_id_via_filter(const char* printer_name ,const char* printer_uri);
//int filter_task_start(const  char* printer_name ,const char* printer_uri);
//int filter_task_end();
#ifdef __cplusplus
}
#endif
#endif // FILTER_CHECK_FINGER_H
