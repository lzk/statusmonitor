#ifndef USBIO_H
#define USBIO_H

#include "usbapi_libusb.h"
#include "deviceio.h"

class UsbIO :public DeviceIO
{
public:
    UsbIO();
    ~UsbIO();

    virtual int type();
    virtual int open(int port = 0);
//    virtual int open(const char* url ,int port = 0);
    virtual int close(void);
    virtual int write(char *buffer, int bufsize);
    virtual int read(char *buffer, int bufsize);
    virtual int resolveUrl(const char* url);
    virtual bool isConnected();
    virtual int getDeviceId(char *buffer, int bufsize);
    virtual const char* getDeviceAddress();
    virtual int write_bulk(char *buffer, int bufsize);
    virtual int read_bulk(char *buffer, int bufsize);

private:
    UsbApi* usb;

private:
    int inPipeRef;
    int outPipeRef;
    int interface;
    int address;
    int vid;
    int pid;
    char serial[256];
};

#endif // USBIO_H
