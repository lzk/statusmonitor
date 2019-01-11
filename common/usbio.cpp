#include "usbio.h"
#include <stdio.h>
#include <string.h>
#include "log.h"
//vid:0x0550 pid:0x0175
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
#if QT_VERSION_MAJOR > 4
#include <QUrlQuery>
#endif
int UsbIO::resolveUrl(const char* url)
{
    int ret = DeviceIO::resolveUrl(url);
    if(ret)
        return ret;
    QString tmp_serial;
#if QT_VERSION_MAJOR > 4
    tmp_serial = QUrlQuery(QUrl(url)).queryItemValue("serial");
    interface = QUrlQuery(QUrl(url)).queryItemValue("interface").toInt();
#else
    tmp_serial = QUrl(url).queryItemValue("serial");
    interface = QUrl(url).queryItemValue("interface").toInt();
#endif
    if(tmp_serial.isEmpty()){
        memset(this->serial ,0 ,sizeof(this->serial));
        ret = -1;
    }else{
        strcpy(this->serial ,tmp_serial.toLatin1().constData());
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
