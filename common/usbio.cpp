#include "usbio.h"
#include <stdio.h>
#include <string.h>
#include "log.h"
#include <QString>
#include "commonapi.h"
#include <unistd.h>

const char* lock_scan_file = "/tmp/.scanner_lock";
const char* lock_scan_info_file = "/tmp/.scanner_info_lock";
int usb_error_printing = -100;
int usb_error_scanning = -101;
int usb_error_usb_locked = -102;

static int _getpidvid(const QString& ,int& pid ,int& vid ,int& interface)
{
    pid = -1;
    vid = -1;
    interface = 0;
    return -1;
}

int (* getpidvid)(const QString& modelname ,int& pid ,int& vid ,int& interface) = _getpidvid;

UsbIO::UsbIO()
    :usb(new UsbApi)
    ,interface(0)
    ,vid(-1)
    ,pid(-1)
{
    memset(this->serial ,0 ,sizeof(this->serial));
    usb->init();
}

UsbIO::~UsbIO()
{
    usb->exit();
    delete usb;
}

int UsbIO::type()
{
    return Type_usb;
}

int UsbIO::resolve(Printer_struct* printer)
{
    printer_name = QString(printer->name);
    return resolveUrl(printer->deviceUri);
}

#include <QSettings>
bool UsbIO::is_device_scanning()
{
//    if(!scanner_locked())
//        return false;

    int ret = lock(lock_scan_file);
    if(!ret){
        unlock();
        return false;
    }

    int bus_NO , device_address;
    QSettings settings(lock_scan_info_file ,QSettings::NativeFormat);
    bus_NO = settings.value("Bus_Number" ,0).toInt();
    device_address = settings.value("Device_Address" ,0).toInt();
    if(!bus_NO || !device_address)
        return false;
//    ret = usb->getDeviceAddress(vid ,pid ,serial ,&address ,&bus);
    if(!ret && address == device_address && bus == bus_NO){
        LOGLOG("usb locked bus:%d ,address:%d" ,bus_NO ,device_address);
        return true;
    }
    return false;
}

#include <QMutex>
#include <QStringList>
static QMutex mutex;
//static int locked_address = -1;
//static int locked_bus = -1;
static QStringList locked_printers;
int UsbIO::open_with_mode(int port ,int mode)
{
    if(device_is_open){
        LOGLOG("device is opened");
        return -1;
    }
    if(port >= 0){
        if(printer_is_printing(printer_name.toLatin1().constData())){
            return usb_error_printing;
        }
    }
    if(is_device_scanning()){
        return usb_error_scanning;
    }
    if(mode == 0){
        while (true) {
            mutex.lock();
            if(!locked_printers.contains(device_uri)){
        //    if(address == locked_address && bus == locked_bus){
                break;
            }
            mutex.unlock();
            usleep(10*1000);
        }
    }else{
        mutex.lock();
        if(locked_printers.contains(device_uri)){
    //    if(address == locked_address && bus == locked_bus){
            mutex.unlock();
            LOGLOG("usb device:%s locked" ,printer_name.toLatin1().constData());
            return usb_error_usb_locked;
        }
    }
    int ret = usb->getDeviceAddress(vid ,pid ,serial ,&address ,&bus);
    if(ret){
        mutex.unlock();
        LOGLOG("can not find device:vid:0x%04x,pid:0x%04x:serial:%s" ,vid ,pid ,serial);
        return -1;
    }
    ret = usb->open(vid ,pid ,serial ,port);
    if(ret){
    }else{
        device_is_open = true;
//        locked_address = address;
//        locked_bus = bus;
        locked_printers << QString(device_uri);
    }
    mutex.unlock();
    return ret;
}

int UsbIO::open(int port)
{
#if 1
    return open_with_mode(port ,0);
#else
    if(device_is_open){
        LOGLOG("device is opened");
        return -1;
    }
    if(port >= 0){
        if(printer_is_printing(printer_name.toLatin1().constData())){
            return usb_error_printing;
        }
    }
    if(is_device_scanning()){
        return usb_error_scanning;
    }
    int ret = usb->getDeviceAddress(vid ,pid ,serial ,&address ,&bus);
    if(ret){
        LOGLOG("can not find device:vid:0x%04x,pid:0x%04x:serial:%s" ,vid ,pid ,serial);
        return -1;
    }
    ret = usb->open(vid ,pid ,serial ,port);
    if(ret){
    }else{
        device_is_open = true;
//        locked_address = address;
//        locked_bus = bus;
    }
    return ret;
#endif
}

int UsbIO::close(void)
{
    if(device_is_open){
        device_is_open = false;
        usb->close();
        mutex.lock();
//        locked_address = -1;
//        locked_bus = -1;
        locked_printers.removeAll(QString(device_uri));
        mutex.unlock();
    }
    return 0;
}

int UsbIO::write(char *buffer, int bufsize)
{
    if(!device_is_open){
        return -1;
    }
    return usb->write(buffer ,bufsize);
}

int UsbIO::read(char *buffer, int bufsize)
{
    if(!device_is_open){
        return -1;
    }
    return usb->read(buffer ,bufsize);
}

int UsbIO::getDeviceId_without_open(char *buffer, int bufsize)
{
    if(!device_is_open){
        return -1;
    }
    return usb->getDeviceId(buffer ,bufsize);
}

int UsbIO::getDeviceId(char *buffer, int bufsize)
{
    int ret = open_with_mode(-1 ,0);
    if(!ret){
        ret = usb->getDeviceId(buffer ,bufsize);
        close();
    }
    return ret;
}

#include <QUrl>
#if QT_VERSION > 0x050000
#include <QUrlQuery>
#endif
int UsbIO::resolveUrl(const char* url)
{
    if(!url)
        return -1;
    if((device_uri[0] != 0) && !QString(device_uri).compare(url)){
        return 0;
    }
    int ret = 0;
    QString tmp_serial;
    QUrl printer_url = QUrl(url);
#if QT_VERSION > 0x050000
    tmp_serial = QUrlQuery(printer_url).queryItemValue("serial");
//    interface = QUrlQuery(printer_url).queryItemValue("interface").toInt();
#else
    tmp_serial = printer_url.queryItemValue("serial");
//    interface = printer_url.queryItemValue("interface").toInt();
#endif
    QString modelname = printer_url.host() + printer_url.path();
    if(getpidvid(modelname ,pid ,vid ,interface)){
        ret = -1;
        LOGLOG("can not get pid vid of modelname:%s" ,modelname.toLatin1().constData());
    }
    if(tmp_serial.isEmpty()){
        memset(this->serial ,0 ,sizeof(this->serial));
        LOGLOG("can not get serial of url:%s" ,url);
    }else{
        strcpy(this->serial ,tmp_serial.toLatin1().constData());
        ret = 0;
    }
    if(!ret){
        ret = DeviceIO::resolveUrl(url);
    }
    return ret;
}

bool UsbIO::isConnected()
{
    bool ret = usb->isConnected(vid ,pid ,serial);
    return ret;
}

const char* UsbIO::getDeviceAddress()
{
    int ret = usb->getDeviceAddress(vid ,pid ,serial ,&address);
    if(ret){
       address = -1;
       return "";
    }
    return QString().sprintf("USB%03d" ,address).toLatin1().constData();
}

int UsbIO::write_bulk(char *buffer, int bufsize ,unsigned int interface)
{
    if(!device_is_open){
        return -1;
    }
    return usb->write_bulk(buffer ,bufsize ,interface);
}

int UsbIO::read_bulk(char *buffer, int bufsize ,unsigned int interface)
{
    if(!device_is_open){
        return -1;
    }
    return usb->read_bulk(buffer ,bufsize ,interface);
}
