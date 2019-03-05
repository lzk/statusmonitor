#include "usbio.h"
#include <stdio.h>
#include <string.h>
#include "log.h"
#include <QString>
#include "commonapi.h"

const char* lock_scan_file = "/tmp/.scanner_lock";
const char* lock_scan_info_file = "/tmp/.scanner_info_lock";
int usb_error_printing = -100;
int usb_error_scanning = -101;
int usb_error_usb_locked = -102;
int usb_error_busy = -103;
extern const char* log_file;
bool printer_is_printing(const QString& printer_name)
{
    QString str("LANG=en lpstat -l -o ");
    str += printer_name;
    str += " 2>>";
    str += log_file;
    str += "|grep -w ";
    str += printer_name;
    QString printer_jobs = get_string_from_shell_cmd(str);
    return !printer_jobs.isEmpty();
}


static int _getpidvid(const QString& ,int* pid ,int* vid)
{
    if(!pid || !vid)
        return -1;
    *pid = -1;
    *vid = -1;
    return -1;
}

int (* getpidvid)(const QString& modelname ,int* pid ,int* vid) = _getpidvid;

#include <QMutex>
#include <QStringList>
static QMutex mutex;
//static int locked_address = -1;
//static int locked_bus = -1;
static QStringList locked_printers;
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

int UsbIO::open(int port)
{
    (void)(port);
    if(device_is_open){
        LOGLOG("device is opened");
        return -1;
    }
    if(printer_is_printing(printer_name.toLatin1().constData())){
        LOGLOG("printer is priting!!!");
        return usb_error_printing;
    }else if(is_device_scanning()){
        return usb_error_scanning;
    }
    int ret = usb->getDeviceAddress(vid ,pid ,serial ,&address ,&bus);
    if(ret){
        LOGLOG("can not find device");
        return -1;
    }
    mutex.lock();
    if(locked_printers.contains(device_uri)){
//    if(address == locked_address && bus == locked_bus){
        mutex.unlock();
        LOGLOG("usb device:%s locked" ,printer_name.toLatin1().constData());
        return usb_error_usb_locked;
    }
    ret = usb->open(vid ,pid ,serial ,interface);
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
    int ret = open();
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
    int ret = DeviceIO::resolveUrl(url);
    if(ret)
        return ret;
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
    if(getpidvid(modelname ,&pid ,&vid))
        ret = -1;
    if(tmp_serial.isEmpty()){
        memset(this->serial ,0 ,sizeof(this->serial));
    }else{
        strcpy(this->serial ,tmp_serial.toLatin1().constData());
        ret = 0;
    }
    if(!ret){
//        LOGLOG("device's vid:0x%02x ,pid:0x%02x ,serial:%s" ,vid ,pid ,serial);
    }else{
        LOGLOG("can not resolve url");
    }
    return ret;
}

bool UsbIO::isConnected()
{
    bool ret = !usb->isConnected(vid ,pid ,serial);
    return ret;
}

const char* UsbIO::getDeviceAddress()
{
    int ret = usb->getDeviceAddress(vid ,pid ,serial ,&address);
    if(ret){
       address = 0;
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
