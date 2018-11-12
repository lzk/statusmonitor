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

    DeviceIO(){}
    virtual ~DeviceIO(){}
    virtual int type() = 0;
    virtual int open(int port = 0) = 0;
    virtual int close(void) = 0;
    virtual int write(char *buffer, int bufsize) = 0;
    virtual int read(char *buffer, int bufsize) = 0;
    virtual bool isConnected() = 0;
    virtual int getDeviceId(char *buffer, int bufsize) = 0;
    virtual const char* getDeviceAddress() = 0;

    virtual int open(const char* url ,int port = 0){
        int ret;
        ret = resolveUrl(url);
        if(!ret)
            ret = open(port);
        return ret;
    }

    virtual int resolveUrl(const char* url){
        int ret = -1;
        if(url){
            strcpy(device_uri ,url);
            ret = 0;
        }
        return ret;
    }

protected:
    char device_uri[256];
};

#endif // DEVICEIO_H
