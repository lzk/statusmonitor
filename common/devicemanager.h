#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QString>
#include "usbio.h"
#include "netio.h"
class DeviceManager
{
public:
    DeviceManager();
    ~DeviceManager();
    DeviceIO* getDevice(const char* uri);

    static int getDeviceType(const char* uri);
private:
    DeviceIO* device;
    UsbIO* usbIO;
    NetIO* netIO;
    QString device_uri;
};

#endif // DEVICEMANAGER_H
