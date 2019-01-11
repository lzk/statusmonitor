#ifndef DEVICEIO_H
#define DEVICEIO_H

#include "string.h"
class DeviceIO
{
public:
    enum{
        Type_unknown,
        Type_usb,
        Type_net,
    };

    DeviceIO():
        ifdelay(1){}
    virtual ~DeviceIO(){}
    virtual int type() = 0;
    virtual int open(int port = 9100) = 0;
    virtual int close(void) = 0;
    virtual int write(char *buffer, int bufsize) = 0;
    virtual int read(char *buffer, int bufsize) = 0;
    virtual bool isConnected() = 0;
    virtual int getDeviceId(char *buffer, int bufsize) = 0;
    virtual int getDeviceId_without_open(char* ,int) = 0;
    virtual const char* getDeviceAddress() = 0;
    virtual int write_bulk(char *buffer, int bufsize ,unsigned int interface=0) = 0;
    virtual int read_bulk(char *buffer, int bufsize ,unsigned int interface=0) = 0;

    virtual int open(const char* url ,int port = 9100);
    virtual int resolveUrl(const char* url);
    virtual int writeThenRead(char* wrBuffer ,int wrSize ,char* rdBuffer ,int rdSize);
protected:
    char device_uri[256];
    int ifdelay;
};

#endif // DEVICEIO_H
