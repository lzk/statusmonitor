#include "scanner.h"
#include "scannerapi.h"
#include "scannerapp.h"

Scanner::Scanner(ScannerApp* app)
    :scanner_app(app)
    ,scannner_api(new ScannerApi(app->device_manager))
{

}

Scanner::~Scanner()
{
    delete scannner_api;
}

extern int usb_error_printing;
extern int usb_error_scanning;
extern int usb_error_usb_locked;
extern int usb_error_busy;
int Scanner::flat_scan(Printer_struct* printer ,ScanSettings* settings)
{
    int ret;
    if(m_cancel)
        return ScannerApp::STATUS_Cancel;

    ret = scannner_api->open(printer);
    if(ret){
        LOGLOG("scanning...error:open");
        return ScannerApp::STATUS_Error_Error;
    }
    ret = scannner_api->lock();
    if(ret){
        LOGLOG("scanning...error:lock");
        scannner_api->close();
        return ret;
    }

    usb_error_usb_locked = usb_error_scanning;
    ret = scannner_api->set_parameters(settings);
    if(ret){
        LOGLOG("scanning...error:set parameters");
        goto ERROR_RETURN;
    }

    ret = scannner_api->start();
    if(ret){
        goto ERROR_RETURN;
    }

    ScanStatus status;
    int size;
    char* buffer;
    int buf_size;
    buffer = settings->info->source_buffer;
    buf_size = settings->info->source_buf_size;
    while(!m_cancel){
        ret = scannner_api->get_scan_status(&status);
        if(ret)
            break;
        if(status.status == ScanStatus_Scanning){
            if(buf_size < status.data_size){
                LOGLOG("not enough buffer");
                ret = -1;
                break;
            }
            size = scannner_api->get_scan_data(buffer ,status.data_size);
//            LOGLOG("scanning...read:%d real:%d" ,status.data_size ,size);
            if(size != status.data_size){
                LOGLOG("scanning...error:get scan para");
                ret = -1;
                break;
            }else{
                scanner_app->save_scan_data(settings ,buffer ,size);
            }
        }else if(status.status == ScanStatus_End){
            ret = status.error_code;
            break;
        }
    }
    if(m_cancel){
        scannner_api->abort();
        ret = ScannerApp::STATUS_Cancel;//cancel
    }

    scannner_api->stop();

 ERROR_RETURN:
    usb_error_usb_locked = usb_error_busy;
    int aa = scannner_api->unlock();
    if(aa){
        LOGLOG("scanner unlock error:%d" ,aa);
    }
    scannner_api->close();
//    if(ret < 0)
//        ret = ScannerApp::STATUS_Error_Error;
    return ret;
}

int Scanner::adf_scan(Printer_struct*  ,ScanSettings*)
{
    return 0;
}
