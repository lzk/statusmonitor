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

#include "filelocker.h"
#include <QSettings>
FileLocker fl;
bool get_locked_sane_scanner(int& address ,int& bus)
{
    address = 0;
    bus = 0;
    QSettings settings(lock_scan_info_file ,QSettings::NativeFormat);
    bus = settings.value("Bus_Number" ,0).toInt();
    address = settings.value("Device_Address" ,0).toInt();
    if(bus && address)
        return true;
    return false;
}

UsbIO::UsbIO()
    :usb(new UsbApi)
    ,interface(0)
    ,vid(-1)
    ,pid(-1)
{
    memset(this->serial ,0 ,sizeof(this->serial));
//    usb->init();
}

UsbIO::~UsbIO()
{
//    usb->exit();
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

#define LOCK_USB 1
#if LOCK_USB
#include <QMutex>
#include <QStringList>
static QMutex mutex;
static QStringList locked_printers;
#endif
int UsbIO::open_with_mode(int port ,int mode)
{
    bool printing = false;
    if(device_is_open){
        LOGLOG("device is opened");
        return -1;
    }
    if(printer_is_printing(printer_name.toLatin1().constData())){
        if(port >= 0)
        return usb_error_printing;
        printing = true;
    }
    if(port < 0)
        port = 0;
#if LOCK_USB
    if(mode == 0){
        while (true) {
            mutex.lock();
            if(locked_printers.isEmpty()){//lock all usb device
//            if(!locked_printers.contains(device_uri)){//lock same device uri
                break;
            }
            mutex.unlock();
            usleep(10*1000);
        }
    }else{
        mutex.lock();
        if(!locked_printers.isEmpty()){//lock all usb device
//        if(locked_printers.contains(device_uri)){//lock same device uri
            mutex.unlock();
            LOGLOG("usb device:%s locked" ,printer_name.toLatin1().constData());
            return usb_error_usb_locked;
        }
    }
    mutex.unlock();
#endif
    int ret = fl.trylock(lock_scan_file);
    int sane_locked_address ,sane_locked_bus;
    if(ret){
        if(get_locked_sane_scanner(sane_locked_address ,sane_locked_bus)){

        }
    }
    usb->init();
    ret = usb->open(vid ,pid ,serial);
    if(!ret){
        usb->getDeviceAddress(&address ,&bus);
        if(address == sane_locked_address && bus == sane_locked_bus){
            LOGLOG("usb locked by sane bus:%d ,address:%d" ,bus ,address);
            usb->close();
            usb->exit();
            fl.unlock();
            return usb_error_scanning;
        }
        if(!printing){
            ret = usb->config(port);
            if(ret){
                usb->close();
            }
        }
    }
    if(!ret){
        device_is_open = true;
#if LOCK_USB
        mutex.lock();
        locked_printers << QString(device_uri);
        mutex.unlock();
#endif
    }else{
        device_is_open = false;
        usb->exit();
        fl.unlock();
    }
    return ret;
}

int UsbIO::open_device(int port ,bool config)
{
    int ret;
    usb->init();
    ret = usb->open(vid ,pid ,serial);
    if(!ret){
        if(config){
            ret = usb->config(port);
            if(ret){
                usb->close();
                usb->exit();
            }
        }
    }else{
        usb->exit();
    }
    return ret;
}
int UsbIO::open(int port)
{
#if 1
    return open_with_mode(port ,0);
#else
    bool config = true;
    if(printer_is_printing(printer_name.toLatin1().constData())){
        if(port >= 0)
            return usb_error_printing;
        config = false;
    }
    if(port < 0){
        port = 0;
    }
    int ret = fl.trylock(lock_scan_file);
    int sane_locked_address ,sane_locked_bus;
    if(ret){// xsane locked
        if(get_locked_sane_scanner(sane_locked_address ,sane_locked_bus)){
            return usb_error_scanning;
        }
    }else{
        ret = open_device(port ,config);
    }
    if(!ret){
        device_is_open = true;
    }else{
        device_is_open = false;
        fl.unlock();
    }
    return ret;
#endif
}

int UsbIO::close(void)
{
    if(device_is_open){
        device_is_open = false;
        usb->close();
        usb->exit();
        fl.unlock();
#if LOCK_USB
        mutex.lock();
        locked_printers.removeAll(QString(device_uri));
        mutex.unlock();
#endif
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
//    int ret = open_with_mode(interface ,0);
    int ret = open(-1);
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
    if((pid != -1) && (device_uri[0] != 0) && !QString(device_uri).compare(url)){
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
    int ret = open(-1);
    if(!ret){
        close();
    }
    return !ret;
}

const char* UsbIO::getDeviceAddress()
{
    int ret = open(-1);
    if(!ret){
        ret = usb->getDeviceAddress(&address);
        close();
    }else{
        address = -1;
        return "";
    }
    return QString().sprintf("USB%03d" ,address).toLatin1().constData();
}

int UsbIO::write_bulk(char *buffer, int bufsize)
{
    if(!device_is_open){
        return -1;
    }
    return usb->write_bulk(buffer ,bufsize);
}

int UsbIO::read_bulk(char *buffer, int bufsize)
{
    if(!device_is_open){
        return -1;
    }
    return usb->read_bulk(buffer ,bufsize);
}
