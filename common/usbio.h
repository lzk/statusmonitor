#ifndef USBIO_H
#define USBIO_H

#include "usbapi_libusb.h"
#include "deviceio.h"
#include "jkinterface.h"
#include <QString>
class UsbIO :public DeviceIO
{
public:
    UsbIO();
    ~UsbIO();

    virtual int type();
//    virtual int open(const char* url ,int port = 0);
    virtual int close(void);
    virtual int write(char *buffer, int bufsize);
    virtual int read(char *buffer, int bufsize);
    virtual int getDeviceId_without_open(char* ,int);
    virtual int write_bulk(char *buffer, int bufsize ,unsigned int interface = 0);
    virtual int read_bulk(char *buffer, int bufsize ,unsigned int interface = 0);

    virtual int resolve(Printer_struct* printer);
protected:
    virtual int open(int port = 0);
    virtual bool isConnected();
    virtual int getDeviceId(char *buffer, int bufsize);
    virtual int resolveUrl(const char* url);
    virtual const char* getDeviceAddress();
private:
    UsbApi* usb;

private:
    int interface;
    int address;
    int vid;
    int pid;
    char serial[256];
    QString printer_name;
};

#endif // USBIO_H
