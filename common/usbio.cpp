#include "usbio.h"
#include <stdio.h>
#include <string.h>
#include "log.h"
//vid:0x0550 pid:0x0175
#include <QString>
static int _getpidvid(const QString& ,int* pid ,int* vid)
{
    if(!pid || !vid)
        return -1;
    *pid = -1;
    *vid = -1;
    return 0;
}

int (* getpidvid)(const QString& modelname ,int* pid ,int* vid) = _getpidvid;

#include <QMutex>
static QMutex mutex;
UsbIO::UsbIO()
    :usb(new UsbApi)
    ,pid(-1)
    ,vid(-1)
    ,interface(0)
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

int UsbIO::open(int port)
{
    (void)(port);
    mutex.lock();
    usb->init();
    int ret = usb->open(vid ,pid ,serial ,interface);
    if(ret){
        usb->exit();
        mutex.unlock();
    }
    return ret;
}

int UsbIO::close(void)
{
    usb->close();
    usb->exit();
    mutex.unlock();
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
    if(tmp_serial.isEmpty()){
        memset(this->serial ,0 ,sizeof(this->serial));
        ret = -1;
    }else{
        strcpy(this->serial ,tmp_serial.toLatin1().constData());
    }
    QString modelname = printer_url.host() + printer_url.path();
    LOGLOG("rosolve printer uri:%s" ,url);
    LOGLOG("model name:%s" ,modelname.toLatin1().constData());
    getpidvid(modelname ,&pid ,&vid);
    LOGLOG("device's vid:0x%02x ,pid:0x%02x ,serial:%s" ,vid ,pid ,serial);
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
