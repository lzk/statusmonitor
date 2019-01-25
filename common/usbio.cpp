#include "usbio.h"
#include <stdio.h>
#include <string.h>
#include "log.h"
#include <QString>
#include "commonapi.h"
#include <sys/file.h>
#include <unistd.h>
#include <sys/stat.h>

const char* lock_scan_file = "/tmp/.scanner_lock";
int error_printing = -100;
int error_scanning = -101;
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

bool scanner_locked()
{
    bool locked = false;
    int altolock = open(lock_scan_file ,O_WRONLY|O_CREAT ,0666);
    fchmod(altolock ,0666);
    if(altolock < 0){
        locked = true;
        LOGLOG("scanner locked by sane driver");
    }else{
        int ret = flock(altolock ,LOCK_EX | LOCK_NB);
        close(altolock);
        locked = !!ret;
    }
    return locked;
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
static QMutex mutex;
UsbIO::UsbIO()
    :usb(new UsbApi)
    ,interface(0)
    ,vid(-1)
    ,pid(-1)
{
    memset(this->serial ,0 ,sizeof(this->serial));
}

UsbIO::~UsbIO()
{
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

int UsbIO::open(int port)
{
    (void)(port);
    if(device_is_open){
        LOGLOG("device is opened");
        return -1;
    }
    if(printer_is_printing(printer_name.toLatin1().constData())){
        return error_printing;
    }else if(scanner_locked()){
        return error_scanning;
    }
    mutex.lock();
    usb->init();
    int ret = usb->open(vid ,pid ,serial ,interface);
    if(ret){
        usb->exit();
        mutex.unlock();
    }else{
        device_is_open = true;
    }
    return ret;
}

int UsbIO::close(void)
{
    if(device_is_open){
        device_is_open = false;
        usb->close();
        usb->exit();
        mutex.unlock();
    }
    return 0;
}

int UsbIO::write(char *buffer, int bufsize)
{
    return usb->write(buffer ,bufsize);
}

int UsbIO::read(char *buffer, int bufsize)
{
    return usb->read(buffer ,bufsize);
}

int UsbIO::getDeviceId_without_open(char *buffer, int bufsize)
{
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
    interface = QUrlQuery(printer_url).queryItemValue("interface").toInt();
#else
    tmp_serial = printer_url.queryItemValue("serial");
    interface = printer_url.queryItemValue("interface").toInt();
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
        LOGLOG("device's vid:0x%02x ,pid:0x%02x ,serial:%s" ,vid ,pid ,serial);
    }else{
        LOGLOG("can not resolve url");
    }
    return ret;
}

bool UsbIO::isConnected()
{
    mutex.lock();
    usb->init();
    bool ret = !usb->isConnected(vid ,pid ,serial);
    usb->exit();
    mutex.unlock();
    return ret;
}

const char* UsbIO::getDeviceAddress()
{
    mutex.lock();
    usb->init();
    int ret = usb->getDeviceAddress(vid ,pid ,serial ,&address);
    usb->exit();
    mutex.unlock();
    if(ret){
       address = 0;
    }
    return QString().sprintf("USB%03d" ,address).toLatin1().constData();
}

int UsbIO::write_bulk(char *buffer, int bufsize ,unsigned int interface)
{
    return usb->write_bulk(buffer ,bufsize ,interface);
}

int UsbIO::read_bulk(char *buffer, int bufsize ,unsigned int interface)
{
    return usb->read_bulk(buffer ,bufsize ,interface);
}
