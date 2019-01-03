#include "scannerapp.h"
#include "scanner.h"
#include "imagetrans.h"
#include "log.h"
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

int trans_init(ScanSettings* settings)
{
    int ret;
    ImageTransInfo* info = settings->info;
    caculate_image_trans_data(settings);

    ret = ImageTrans::init(info);
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
    ret = ImageTrans::trans(info);
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
}

static void image_trans_thread(void *reference)
{
    int ret;
    ScanSettings* settings = (ScanSettings*)reference;
    ImageTransInfo* info = settings->info;
    ret = trans_init(settings);
    if(ret < 0)
        return;
    while (1) {
        ret = ImageTrans::trans(info);
        if(ret)
            break;
    }
    trans_exit(settings);
}


ScannerApp::ScannerApp(DeviceIO** _device)
    :device(_device)
{
    scanner = new Scanner(this);
}

ScannerApp::~ScannerApp()
{
    delete scanner;
}

int ScannerApp::init_scan(ScanSettings* settings)
{
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

int ScannerApp::save_scan_data(ScanSettings* settings)
{
    trans_trans(settings);
    return 0;
}

int ScannerApp::exit_scan(ScanSettings* settings)
{
    trans_exit(settings);
    return 0;
}

#define SCAN_BUFFER_SIZE 	0x10000
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

void ScannerApp::set_cancel(bool cancel)
{
    scanner->set_cancel(cancel);
}
