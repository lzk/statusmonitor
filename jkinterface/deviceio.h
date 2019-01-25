#ifndef DEVICEIO_H
#define DEVICEIO_H

struct Printer_struct;
class DeviceIO
{
public:
    enum{
        Type_unknown,
        Type_usb,
        Type_net,
    };

    DeviceIO()
        :ifdelay(1)
      ,device_is_open(false){}
    virtual ~DeviceIO(){}
    virtual int type() = 0;
    virtual int close(void) = 0;
    virtual int write(char *buffer, int bufsize) = 0;
    virtual int read(char *buffer, int bufsize) = 0;
    virtual int getDeviceId_without_open(char* ,int) = 0;
    virtual int write_bulk(char *buffer, int bufsize ,unsigned int interface=0) = 0;
    virtual int read_bulk(char *buffer, int bufsize ,unsigned int interface=0) = 0;

    virtual int writeThenRead(char* wrBuffer ,int wrSize ,char* rdBuffer ,int rdSize);

    virtual int open(Printer_struct* printer ,int port);
    virtual int resolve(Printer_struct* printer);
    virtual bool isConnected(Printer_struct* printer);
    virtual const char* getDeviceAddress(Printer_struct* printer);
    virtual int getDeviceId(Printer_struct* printer ,char *buffer, int bufsize);
protected:
    char device_uri[256];
    int ifdelay;
    bool device_is_open;
    virtual int open(int port) = 0;
    virtual int resolveUrl(const char* url);
    virtual bool isConnected() = 0;
    virtual int getDeviceId(char *buffer, int bufsize) = 0;
    virtual const char* getDeviceAddress() = 0;
};

class DeviceIOManager
{
public:
    virtual ~DeviceIOManager(){}
    virtual DeviceIO* getDevice(Printer_struct* printer) = 0;
};

#endif // DEVICEIO_H
