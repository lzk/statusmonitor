#ifndef USBAPI_LIBUSB_H
#define USBAPI_LIBUSB_H

#include "libusb.h"
typedef struct struct_deviceInfo{
    int vid;
    int pid;
    int locationID;
    int address;
    char serial[256];
}
    struct_deviceInfo;

typedef struct struct_deviceList{
    struct_deviceInfo* deviceList;
    int length;
    int maxLength;
}
    struct_deviceList;

typedef struct struct_device{
    struct_deviceInfo deviceInfo;
    libusb_device* dev;
    libusb_device_handle *udev;
}
    struct_device;

typedef struct struct_deviceInterface{
    int interface;
    int inPipeRef;
    int outPipeRef;
//    IOUSBInterfaceInterface_version **intf;
}
    struct_deviceInterface;
typedef void (* addUsbDevice)(struct_deviceInfo* ,void*);
typedef int (* usbDeviceHandler)(libusb_device* ,void*);

class UsbApi
{
public:
    UsbApi();
    int init();
    int exit();
    int getDeviceId(char* ,int);
    int open(int vid ,int pid ,const char* serial ,int interface = 0);
    int close();
    int getDeviceWithSerial(int vid ,int pid ,const char* serial);
    int write(char* buffer ,int bufsize);
    int read(char* buffer ,int bufsize);
    bool isConnected(int vid, int pid, const char *serial);
    int getDeviceAddress(int vid, int pid, const char *serial ,int* address ,int* bus = 0);

    int write_bulk(char* buffer ,int bufsize ,unsigned int interface = 0);
    int read_bulk(char* buffer ,int bufsize ,unsigned int interface = 0);

    static int iterateDevices(usbDeviceHandler handler ,void* handlerData);
private:
    int getDeviceWithSerial(struct_device*);
    int config(libusb_device *dev ,libusb_device_handle *udev);

private:
    int g_interface;
    libusb_device *g_device;
    libusb_device_handle *g_dev_h;
    int bulk_in[2];
    int bulk_out[2];
};

#endif // USBAPI_LIBUSB_H
