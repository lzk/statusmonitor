#include "scanner.h"
#include "scannerapi.h"
#include "scannerapp.h"

Scanner::Scanner(ScannerApp* app)
    :scanner_app(app)
    ,scannner_api(new ScannerApi(app->device))
{

}

Scanner::~Scanner()
{
    delete scannner_api;
}

int Scanner::flat_scan(ScanSettings* settings)
{
    int ret;
    if(m_cancel)
        return ScannerApp::STATUS_Cancel;

    ret = scannner_api->open();
    if(ret){
        LOGLOG("scanning...error:open");
        return ScannerApp::STATUS_Error_Error;
    }
    ret = scannner_api->lock();
    if(ret){
        LOGLOG("scanning...error:lock");
        scannner_api->close();
        return ScannerApp::STATUS_Error_Error;
    }

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
                settings->info->scanned_buf_size = size;
                scanner_app->save_scan_data(settings);
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
    scannner_api->unlock();
    scannner_api->close();
    if(ret < 0)
        ret = ScannerApp::STATUS_Error_Error;
    return ret;
}

int Scanner::adf_scan(ScanSettings*)
{
    return 0;
}