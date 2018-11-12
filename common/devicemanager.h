#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "usbio.h"
#include "netio.h"
class DeviceManager
{
public:
    DeviceManager();
    DeviceIO* getDevice(const char* uri);

    static int getDeviceType(const char* uri);
private:
    DeviceIO* device;
    NetIO netIO;
    UsbIO usbIO;
    QString device_uri;
};

#endif // DEVICEMANAGER_H
