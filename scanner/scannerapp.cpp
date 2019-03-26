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

FILE* cache_file = NULL;
FILE* source_file = NULL;
const char* jpg_file_name = "/tmp/tmpscan.jpg";
const char* raw_file_name = "/tmp/tmpscan.raw";
int cache_buffer_init(const char* mode ,int type=0);
int cache_buffer_init(const char* mode ,int type)
{
    int ret = 0;
    const char* cache_file_name;
    switch (type) {
    case ImageTransFormat_raw:
        cache_file_name = raw_file_name;
        break;
    case ImageTransFormat_jpg:
        cache_file_name = jpg_file_name;
        break;
    default:
        cache_file_name = NULL;
        break;
    }
    cache_file = NULL;
    if(cache_file_name){
        cache_file = fopen(cache_file_name ,mode);
    }
    if(!cache_file){
        ret = -1;
    }
    return ret;
}
int cache_buffer_write(char* buffer ,int buf_size)
{
    if(!cache_file)
        return -1;
    return fwrite(buffer ,sizeof(char) ,buf_size ,cache_file);
}
int cache_buffer_read(char* buffer ,int buf_size)
{
    if(!cache_file)
        return -1;
    return fread(buffer ,sizeof(char) ,buf_size ,cache_file);
}
int cache_buffer_exit()
{
    if(!cache_file)
        return -1;
    fclose(cache_file);
    cache_file = NULL;
    return 0;
}
int source_buffer_init(const char* mode)
{
    int ret = 0;
    source_file = fopen(raw_file_name ,mode);
    if(!source_file){
        ret = -1;
    }
    return ret;
}

int source_buffer_read(char* buffer ,int buf_size)
{
    if(!source_file)
        return -1;
    return fread(buffer ,sizeof(char) ,buf_size ,source_file);
}

int source_buffer_write(char* buffer ,int buf_size)
{
    if(!source_file)
        return -1;
    return fwrite(buffer ,sizeof(char) ,buf_size ,source_file);
}

int source_buffer_exit()
{
    if(!source_file)
        return -1;
    fclose(source_file);
    source_file = NULL;
    return 0;
}

//int trans_init(ScanSettings* settings)
//{
//    int ret;
//    ImageTransInfo* info = settings->info;

//    caculate_image_trans_data(settings);

//    ret = settings->info->image_trans->init(info);
//    if(ret <= 0){
//        return ret;
//    }
//    info->target_buf_size = ret;

//    settings->file = fopen(settings->filename ,"w+b");
//    if(!settings->file)
//        ret = -1;
//    //save head
//    else{
//        fwrite(info->target_buffer ,sizeof(char) ,info->target_buf_size ,settings->file);
//        fflush(settings->file);
//    }
//    return ret;
//}

//int trans_trans(ScanSettings* settings)
//{
//    ImageTransInfo* info = settings->info;
//    int ret;
//    ret = settings->info->image_trans->trans(info);
//    //save image
//    if(settings->file){
//        fwrite(info->target_buffer ,sizeof(char) ,info->target_buf_size ,settings->file);
//        fflush(settings->file);
//    }
//    return ret;
//}

//void trans_exit(ScanSettings* settings)
//{
//    if(settings->file){
//        fclose(settings->file);
//        settings->file = NULL;
//    }
//    settings->info->image_trans->exit();
//}

//static void image_trans_thread(void *reference)
//{
//    int ret;
//    ScanSettings* settings = (ScanSettings*)reference;
//    ret = trans_init(settings);
//    if(ret < 0)
//        return;
//    while (1) {
//        ret = trans_trans(settings);
//        if(ret)
//            break;
//    }
//    trans_exit(settings);
//}


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

//int ScannerApp::init_scan(ScanSettings* settings)
//{
//    settings->info->image_trans = image_trans;
//    calculate_parameters(settings);
//    int ret = trans_init(settings);
//    if(ret < 0)
//        return ret;
//    else
//        ret = 0;

////    if (pthread_create(&thread, NULL, image_trans_thread, (void*)settings))
////    {
////        ret = -1;
////    }

//    return ret;
//}

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
    cache_buffer_write(buffer ,buf_size);
    return 0;
}

//int ScannerApp::exit_scan(ScanSettings* settings)
//{
//    trans_exit(settings);
//    return 0;
//}

#define SCAN_BUFFER_SIZE 	0x10000
int ScannerApp::trans_process(ScanSettings* settings)
{
    int ret;
    ret = source_buffer_init("rb");
    if(ret){
        return ret;
    }

    ImageTransInfo* info = settings->info;

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
        source_buffer_read(buffer ,each_source_size);
        info->each_source_size = each_source_size;
        info->lines = lines;
        image_trans->process(info);
    }
    source_buffer_exit();
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
    caculate_image_trans_data(settings);
#if Test_Jerry
    ImageTransInfo* info = settings->info;
    char* source_buf = new char[SCAN_BUFFER_SIZE];
    info->source_buffer = source_buf;
    info->source_buf_size = SCAN_BUFFER_SIZE;
    settings->file = NULL;

    ret = cache_buffer_init("wb+" ,info->source_format);
//    ret = init_scan(settings);
    if(ret){
        return STATUS_Error_App;
    }

    set_cancel(false);
    settings->received_bytes = 0;
    usb_error_usb_locked = usb_error_scanning;
    for(int i = 0 ;i < 3 ;i++){
        ret = scanner->flat_scan(printer ,settings);
        if(ret != STATUS_Error_App && ret != STATUS_Error_lock && ret != STATUS_Error_busy)
            break;
        else
            usleep(100 * 1000);
    }
    if(STATUS_Error_lock == ret)
        ret = STATUS_Error_machine;
    usb_error_usb_locked = usb_error_busy;
//    exit_scan(settings);
    cache_buffer_exit();
    delete [] source_buf;
#endif
    if(!ret){
        trans_source_buffer(info->source_format);
        trans_process(settings);
        settings->progress  = -2;
        if(settings->callback)
            settings->callback(settings);
    }else{
        settings->progress  = -3;
        if(settings->callback)
            settings->callback(settings);
    }
    return ret;
}

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

#include "trans_jpg.h"
int ScannerApp::trans_source_buffer(int type)
{
    int ret = 0;
    if(type == ImageTransFormat_jpg){
        //transfer jpg to raw
        source_buffer_init("wb+");
        read_JPEG_file((char*)jpg_file_name ,source_buffer_write);
        source_buffer_exit();
    }
    return ret;
}