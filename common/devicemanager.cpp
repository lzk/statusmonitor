#include "devicemanager.h"
#include "jkinterface.h"
DeviceManager::DeviceManager()
    :device(NULL)
    ,usbIO(new UsbIO)
    ,netIO(new NetIO)
{

}

DeviceManager::~DeviceManager()
{
    delete usbIO;
    delete netIO;
}

DeviceIO* DeviceManager::getDevice(const char* device_uri)
{
    int type = getDeviceType(device_uri);
    switch (type) {
    case DeviceIO::Type_usb:
        device = usbIO;
        break;

    case DeviceIO::Type_net:
        device = netIO;
        break;

    default:
        device = NULL;
        break;
    }
    if(this->device_uri.compare(device_uri)){
//        LOGLOG("update device:%s" ,device_uri);
        this->device_uri = device_uri;
        if(device){
            device->resolveUrl(device_uri);
        }
    }
    return device;
}

int DeviceManager::getDeviceType(const char* device_uri)
{
    int type = DeviceIO::Type_unknown;
    if(!strncmp(device_uri,"usb://" ,6)){
        type = DeviceIO::Type_usb;
    }else{
        type = DeviceIO::Type_net;
    }
    return type;
}
