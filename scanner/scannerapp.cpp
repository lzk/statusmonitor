#include "scannerapp.h"
#include "scanner.h"
#include "imagetrans.h"
#include "log.h"
#include <unistd.h>
void caculate_image_trans_data(ScanSettings* settings);
void calculate_parameters(ScanSettings* scan_settings);

//const char* scan_tmp_file = "/tmp/.scan_tmp";
//const ScanSettings default_scan_settings = {
//    .scan_doctype = T_Photo,
//    .scan_dpi = Scan_300DPI,
//    .colorModel = Color,
//    .contrast = 50,
//    .brightness = 50,
//    .scan_size = Scan_A4,
//};

const char* scan_tmp_file_name = "/tmp/tmpscan.raw";
FILE* scan_file;
int scan_buffer_open(const char* mode)
{
    int ret = 0;
    scan_file = fopen(scan_tmp_file_name ,mode);
    if(!scan_file){
        ret = -1;
    }
    return ret;
}

int scan_buffer_read(char* buffer ,int buf_size)
{
    return fread(buffer ,sizeof(char) ,buf_size ,scan_file);
}

int scan_buffer_write(char* buffer ,int buf_size)
{
    return fwrite(buffer ,sizeof(char) ,buf_size ,scan_file);
}

int scan_buffer_exit()
{
    fclose(scan_file);
    scan_file = NULL;
}

int trans_init(ScanSettings* settings)
{
    int ret;
    ImageTransInfo* info = settings->info;

    caculate_image_trans_data(settings);

    ret = settings->info->image_trans->init(info);
    if(ret <= 0){
        return ret;
    }
    info->target_buf_size = ret;

    settings->file = fopen(settings->filename ,"w+b");
    if(!settings->file)
        ret = -1;
    //save head
    else{
        fwrite(info->target_buffer ,sizeof(char) ,info->target_buf_size ,settings->file);
        fflush(settings->file);
    }
    return ret;
}

int trans_trans(ScanSettings* settings)
{
    ImageTransInfo* info = settings->info;
    int ret;
    ret = settings->info->image_trans->trans(info);
    //save image
    if(settings->file){
        fwrite(info->target_buffer ,sizeof(char) ,info->target_buf_size ,settings->file);
        fflush(settings->file);
    }
    return ret;
}

void trans_exit(ScanSettings* settings)
{
    if(settings->file){
        fclose(settings->file);
        settings->file = NULL;
    }
    settings->info->image_trans->exit();
}

static void image_trans_thread(void *reference)
{
    int ret;
    ScanSettings* settings = (ScanSettings*)reference;
    ret = trans_init(settings);
    if(ret < 0)
        return;
    while (1) {
        ret = trans_trans(settings);
        if(ret)
            break;
    }
    trans_exit(settings);
}


ScannerApp::ScannerApp(DeviceIOManager* _device_manager)
    :device_manager(_device_manager)
{
    scanner = new Scanner(this);
    image_trans = new ImageTrans;
}

ScannerApp::~ScannerApp()
{
    delete scanner;
    delete image_trans;
}

int ScannerApp::init_scan(ScanSettings* settings)
{
    settings->info->image_trans = image_trans;
    calculate_parameters(settings);
    int ret = trans_init(settings);
    if(ret < 0)
        return ret;
    else
        ret = 0;

//    if (pthread_create(&thread, NULL, image_trans_thread, (void*)settings))
//    {
//        ret = -1;
//    }

    return ret;
}

int ScannerApp::save_scan_data(ScanSettings* settings ,char* buffer ,int buf_size)
{
//    trans_trans(settings);
    settings->received_bytes += buf_size;
    if(settings->calc_data.source_size){
        settings->progress = 100 * settings->received_bytes / settings->calc_data.source_size;
//        LOGLOG("%ld/%ld = %d" ,settings->received_bytes ,settings->calc_data.source_size ,settings->progress);
    }
    if(settings->callback)
        settings->callback(settings);
    scan_buffer_write(buffer ,buf_size);
    return 0;
}

int ScannerApp::exit_scan(ScanSettings* settings)
{
    trans_exit(settings);
    return 0;
}

#define SCAN_BUFFER_SIZE 	0x10000
#if 0
int ScannerApp::scan(ScanSettings* settings)
{
    int ret;
    char* source_buf = new char[SCAN_BUFFER_SIZE];
    char* target_buf = new char[SCAN_BUFFER_SIZE];
    char* resume_buf = new char[SCAN_BUFFER_SIZE];
    ImageTransInfo image_trans_info;
    ImageTransInfo* info = settings->info = &image_trans_info;
    info->resume_buffer = resume_buf;
    info->resume_buf_size = SCAN_BUFFER_SIZE;
    info->source_buffer = source_buf;
    info->source_buf_size = SCAN_BUFFER_SIZE;
    info->target_buffer = target_buf;
    info->target_buf_size = SCAN_BUFFER_SIZE;
    settings->file = NULL;

    ret = init_scan(settings);
    if(ret){
        return STATUS_Error_App;
    }
    set_cancel(false);
    ret = scanner->flat_scan(settings);
    exit_scan(settings);
    delete [] source_buf;
    delete [] target_buf;
    delete [] resume_buf;
    return ret;
}
#else

int ScannerApp::trans_process(ScanSettings* settings)
{
    int ret;
    ret = scan_buffer_open("rb");
    if(ret){
        return ret;
    }

    ImageTransInfo* info = settings->info;
    caculate_image_trans_data(settings);


    int each_lines = 3;
    int each_source_size =  each_lines * info->source_line_buf_size;
    char* buffer = new char[each_source_size];
    info->scan_buffer = buffer;

//    int source_size = each_lines * info->source_line_buf_size;
    char* jerry_buffer = new char[each_source_size * 3];
    info->resume_buffer = jerry_buffer;
    info->resume_buf_size = each_source_size * 3;

    int target_lines = each_lines * 10 / info->source_lines_per_10_lines;
    int target_size = target_lines * info->target_line_buf_size;
    char* target_buffer = new char[target_size];
    info->target_buffer = target_buffer;
    info->target_buf_size = target_size;

    info->filename = (const char*)settings->filename;
    image_trans->init(info);
    int lines;
    for(int rest_lines = info->source_total_lines ;rest_lines > 0 ;rest_lines = rest_lines - each_lines){
        if(rest_lines > each_lines){
            lines = each_lines;
        }else{
            lines = rest_lines;
        }
        each_source_size = lines * info->source_line_buf_size;
        scan_buffer_read(buffer ,each_source_size);
        info->each_source_size = each_source_size;
        info->lines = lines;
        image_trans->process(info);
    }
    scan_buffer_exit();
    image_trans->exit();

    delete [] buffer;
    delete [] jerry_buffer;
    delete [] target_buffer;
    return ret;
}

#define Test_Jerry 1
extern int usb_error_printing;
extern int usb_error_scanning;
extern int usb_error_usb_locked;
extern int usb_error_busy;
int ScannerApp::scan(Printer_struct* printer ,ScanSettings* settings)
{
    int ret = 0;
    settings->printer = printer;
    ImageTransInfo image_trans_info;
    settings->info = &image_trans_info;
    calculate_parameters(settings);
#if Test_Jerry
    ImageTransInfo* info = settings->info;
    char* source_buf = new char[SCAN_BUFFER_SIZE];
    info->source_buffer = source_buf;
    info->source_buf_size = SCAN_BUFFER_SIZE;
    settings->file = NULL;

    ret = scan_buffer_open("wb+");
//    ret = init_scan(settings);
    if(ret){
        return STATUS_Error_App;
    }

    set_cancel(false);
    settings->received_bytes = 0;
    usb_error_usb_locked = usb_error_scanning;
    for(int i = 0 ;i < 3 ;i++){
        ret = scanner->flat_scan(printer ,settings);
        if(ret != STATUS_Error_App)
            break;
        else
            usleep(100 * 1000);
    }
    usb_error_usb_locked = usb_error_busy;
//    exit_scan(settings);
    scan_buffer_exit();
    delete [] source_buf;
#endif
    if(!ret){
        settings->progress  = -2;
        if(settings->callback)
            settings->callback(settings);
        trans_process(settings);
    }else{
        settings->progress  = -3;
        if(settings->callback)
            settings->callback(settings);
    }
    return ret;
}
#endif
void ScannerApp::set_cancel(bool cancel)
{
    scanner->set_cancel(cancel);
}

int ScannerApp::start_scan(ScanSettings* settings)
{
    settings->progress = -1;//start
    if(settings->callback)
        settings->callback(settings);
}
