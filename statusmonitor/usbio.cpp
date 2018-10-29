#include "usbio.h"
#include <stdio.h>
#include "log.h"
void UsbIO::test()
{
    LOGLOG("test2\n");

//    usb->getDeviceWithSerial(0x0550 ,0x0175 ,"010623--CE");
//    usb->getDeviceWithSerial(0x0550 ,0x0175 ,NULL);

    if(!open(0x0550 ,0x0175 ,NULL)){
        char buffer[1024];
        getDeviceId(buffer ,sizeof(buffer));
        close();
        LOGLOG("device id is:\n\t%s\n" ,buffer);
    }
}

UsbIO::UsbIO()
    :usb(&usbapi)
{
    usb->init();
}

UsbIO::~UsbIO()
{
    usb->exit();
}

int UsbIO::open(int vid ,int pid ,const char* serial)
{
    return usb->open(vid ,pid ,serial);
}
int UsbIO::close(void)
{
    usb->close();
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

int UsbIO::getDeviceId(char *buffer, int bufsize)
{
    return usb->getDeviceId(buffer ,bufsize);
}

bool UsbIO::isConnected(int vid ,int pid ,const char* serial)
{
    return !usb->getDeviceWithSerial(vid ,pid ,serial);
}
