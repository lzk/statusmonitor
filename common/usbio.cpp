#include "usbio.h"
#include <stdio.h>
#include <string.h>
#include "log.h"
#include <QString>
#include "commonapi.h"
#include <unistd.h>

const char* lock_scan_file = "/tmp/.tjgd_lock";
const char* lock_scan_info_file = "/tmp/.tjgd_info_lock";
int usb_error_printing = -100;
int usb_error_scanning = -101;
int usb_error_usb_locked = -102;
int usb_error_busy = -103;

static int _getpidvid(const QString& ,int* pid ,int* vid)
{
    if(!pid || !vid)
        return -1;
    *pid = -1;
    *vid = -1;
    return -1;
}

int (* getpidvid)(const QString& modelname ,int* pid ,int* vid) = _getpidvid;

#include "filelocker.h"
#include <QSettings>
FileLocker fl;
bool get_locked_sane_scanner(int& address ,int& bus)
{
    address = 0;
    bus = 0;
//    FileLocker fl;
//    int ret = fl.trylock(lock_scan_file);
//    if(!ret){
       // fl.unlock();

        QSettings settings(lock_scan_info_file ,QSettings::NativeFormat);
        bus = settings.value("Bus_Number" ,0).toInt();
        address = settings.value("Device_Address" ,0).toInt();
        if(bus && address)
//             fl.unlock();
            return true;
//    }
//    fl.unlock();
    return false;
}

bool get_locked_device(QString& uri)
{
    uri.clear();
//    FileLocker fl;
    int ret = fl.trylock(lock_scan_file);
    if(ret){
        QSettings settings(lock_scan_info_file ,QSettings::NativeFormat);
        uri = settings.value("Device_Uri").toString();
        if(!uri.isEmpty())
            return true;
    }

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
    if(device_is_open){
        LOGLOG("device is opened");
        return -1;
    }
    if(port >= 0 && mode >= 0){
        if(printer_is_printing(printer_name.toLatin1().constData())){
            return usb_error_printing;
        }
    }
    QString locked_uri;
    if(get_locked_device(locked_uri)){
        if(!locked_uri.compare(this->device_uri)){
            return usb_error_busy;
        }
    }
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
            fl.unlock();
            return usb_error_usb_locked;
        }
    }
    mutex.unlock();
#endif
//    int sane_locked_address ,sane_locked_bus;
//    if(get_locked_sane_scanner(sane_locked_address ,sane_locked_bus)){
//        if(address == sane_locked_address && bus == sane_locked_bus){
//            LOGLOG("usb locked bus:%d ,address:%d" ,bus ,address);
//            return usb_error_scanning;
//        }
//    }
    usb->init();
    int ret = usb->open(vid ,pid ,serial);
    if(!ret){
        usb->getDeviceAddress(&address ,&bus);
//        if(address == sane_locked_address && bus == sane_locked_bus){
//            LOGLOG("usb locked by sane bus:%d ,address:%d" ,bus ,address);
//            usb->close();
//            usb->exit();
//            return usb_error_scanning;
//        }
        if(port >= 0){
            ret = usb->config(port);
            if(ret){
                usb->close();
                usb->exit();
            }
        }
    }else{
        usb->exit();
    }
    if(!ret){
        device_is_open = true;
#if LOCK_USB
        mutex.lock();
        locked_printers << QString(device_uri);
        mutex.unlock();
#endif
    }else{
        fl.unlock();
    }
    return ret;
}

int UsbIO::open(int port)
{
    return open_with_mode(port ,0);
}

int UsbIO::close(void)
{
    if(device_is_open){
        device_is_open = false;
        usb->close();
        usb->exit();
#if LOCK_USB
        mutex.lock();
        locked_printers.removeAll(QString(device_uri));
        mutex.unlock();
#endif

    }
    fl.unlock();
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
#if 0
int UsbIO::getDeviceId(char *buffer, int bufsize)
{
    //return -1;//for debug
    int ret = open_with_mode(1 ,-1);//will change to -1
    if(!ret){
        char cmd[12];
        memset(cmd ,0 ,sizeof(cmd));
        cmd[3] = 0x1b;
        cmd[2] = 0x49;
        cmd[1] = 0x46;
        cmd[0] = 0x32;
        cmd[4] = 1;
        cmd[5] = 1;
        cmd[6] = 0xff;
        cmd[7] = 0xff;

        ret = write_bulk(cmd ,sizeof(cmd));
        if(ret == sizeof(cmd)){
            ret = read_bulk(buffer ,bufsize);
            LOGLOG("read bulk length:%d" ,ret);
            LOGLOG("respond:0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x"
                   ",0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x"
                   ,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]
                    ,buffer[6],buffer[7],buffer[8],buffer[9],buffer[10],buffer[11]);
            if((ret == 12)
                    && (buffer[3] == 0x1b)
                    && (buffer[2] == 0x49)
                    && (buffer[1] == 0x46)
                    && (buffer[0] == 0x32)
                    && (buffer[4] == 1) //group
                    && (buffer[5] == 1) //code
                    && (buffer[6] == 0) //status
                    && (buffer[7] == 0) //status
                    )
            {
                int length = *((int*)(buffer + 8));
                LOGLOG("payload length:%d" ,length);
                    strcpy(buffer ,"STS:");
                   ret = read_bulk(buffer + 4 ,length);
                   if(ret == length){
                       ret = 0;
                   }
                   else{
                       ret = -1;
                   }
            }else{
                ret = -1;
            }
        }else{
            ret = -1;
        }

        close();
    }
    return ret;
}
#else
int UsbIO::getDeviceId(char *buffer, int bufsize)
{
    //return -1;//for debug

    int ret = open_with_mode(0 ,0);//will change to -1
    if(!ret){
        ret = usb->getDeviceId(buffer ,bufsize);
        close();
    }
    return ret;
}
#endif
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
    if(getpidvid(modelname ,&pid ,&vid)){
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
    int ret = open_with_mode(-1 ,0);
    if(!ret){
        close();
    }
    return !ret;
}

const char* UsbIO::getDeviceAddress()
{
    int ret = open_with_mode(-1 ,0);
    if(!ret){
        ret = usb->getDeviceAddress(&address);
        close();
    }else{
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
