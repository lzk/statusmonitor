#ifndef USBIO_H
#define USBIO_H

#include "usbapi_libusb.h"
class UsbIO
{
public:
    UsbIO();
    ~UsbIO();
    int open(int vid ,int pid ,const char* serial);
    int close(void);
    int write(char *buffer, int bufsize);
    int read(char *buffer, int bufsize);
    int getDeviceId(char *buffer, int bufsize);

    bool isConnected(int vid ,int pid ,const char* serial);

    void test();

private:
    UsbApi* usb;
    UsbApi usbapi;
};

#endif // USBIO_H
