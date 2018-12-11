#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QString>
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
    QString device_uri;
    UsbIO usbIO;
    NetIO netIO;
};

#endif // DEVICEMANAGER_H
