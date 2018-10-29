#include "usbapi_libusb.h"
#include <stdio.h>
#include <string.h>
#include "log.h"

extern "C" {
int print_device(libusb_device *dev, int level);
}

static const char* my_libusb_strerror(int errcode)
{
  /* Error codes & descriptions from the libusb-1.0 documentation */

  switch (errcode)
    {
      case LIBUSB_SUCCESS:
    return "Success (no error)";

      case LIBUSB_ERROR_IO:
    return "Input/output error";

      case LIBUSB_ERROR_INVALID_PARAM:
    return "Invalid parameter";

      case LIBUSB_ERROR_ACCESS:
    return "Access denied (insufficient permissions)";

      case LIBUSB_ERROR_NO_DEVICE:
    return "No such device (it may have been disconnected)";

      case LIBUSB_ERROR_NOT_FOUND:
    return "Entity not found";

      case LIBUSB_ERROR_BUSY:
    return "Resource busy";

      case LIBUSB_ERROR_TIMEOUT:
    return "Operation timed out";

      case LIBUSB_ERROR_OVERFLOW:
    return "Overflow";

      case LIBUSB_ERROR_PIPE:
    return "Pipe error";

      case LIBUSB_ERROR_INTERRUPTED:
    return "System call interrupted (perhaps due to signal)";

      case LIBUSB_ERROR_NO_MEM:
    return "Insufficient memory";

      case LIBUSB_ERROR_NOT_SUPPORTED:
    return "Operation not supported or unimplemented on this platform";

      case LIBUSB_ERROR_OTHER:
    return "Other error";

      default:
    return "Unknown libusb-1.0 error code";
    }

  return "Unknown libusb-1.0 error code";
}


int iterateDevices(usbDeviceHandler handler ,void* handlerData)
{
    int ret = -1;
    libusb_device **devlist;
    ssize_t ndev;
    ndev = libusb_get_device_list (NULL, &devlist);
    if(ndev < 0)
        return (int)ndev;

    libusb_device *dev;
    int i;
    for (i = 0; i < ndev; i++){
        dev = devlist[i];
#if DEBUG
      print_device(dev ,0);
#endif
        if(handler){
            ret = handler(dev ,handlerData);
            if(!ret)
                break;
        }
    }
    libusb_free_device_list(devlist, 1);
    return ret;
}

//static bool get_serial(libusb_device* dev ,char* serial)
static bool get_serial(libusb_device*  ,char* )
{
    return true;
}

static int _getUsbDeviceWithSerail(libusb_device* dev ,void* pData)
{
    struct_device* pData_device = (struct_device*)pData;
    pData_device->dev = NULL;

    if(!pData || !dev)
        return -1;
    int ret;
    struct libusb_device_descriptor desc;
    libusb_device_handle *udev = NULL;

    ret = libusb_get_device_descriptor (dev, &desc);
    if (ret < 0){
        return -1;
    }
    ret = libusb_open (dev, &udev);
    if(ret < 0)
        return -1;
    char devserialNumber[1024];
    if (desc.iSerialNumber){
        ret = libusb_get_string_descriptor_ascii(udev, desc.iSerialNumber, (unsigned char *)devserialNumber, sizeof(devserialNumber));
#if DEBUG
        if (ret > 0)
            LOGLOG("%.*s  - Serial Number: %s\n", 0,
            "                    ", devserialNumber);
#endif
    }

    get_serial(dev ,devserialNumber);
    if(pData_device->deviceInfo.vid == desc.idVendor
            && pData_device->deviceInfo.pid == desc.idProduct){
        if(pData_device->deviceInfo.serial[0] == NULL || !strcmp(pData_device->deviceInfo.serial ,devserialNumber)){
#if DEBUG
            LOGLOG("found success ,break searching round\n");
#endif
            pData_device->dev = dev;
            pData_device->udev = udev;
            ret = 0;
            return ret;
        }else{
            ret = 1;
        }
    }
    if(udev)
        libusb_close(udev);
    return ret;
}

int config(libusb_device *dev ,libusb_device_handle *udev)
{
    int result;

    int config;
    struct libusb_device_descriptor desc;
    struct libusb_config_descriptor *config0;

    result = libusb_get_configuration (udev, &config);
    if (result < 0){
        LOGLOG("get_device_id: Could not get configuration for device (err %d)\n", result);
        return result;
    }

    if (config == 0){
        LOGLOG("get_device_id: Device not configured?\n");
        return 1;
    }

    result = libusb_get_device_descriptor (dev, &desc);
    if (result < 0)
    {
        LOGLOG("get_device_id: Could not get device descriptor for device  (err %d)\n", result);
        return result;
    }

    result = libusb_get_config_descriptor (dev, 0, &config0);
    if (result < 0)
    {
        LOGLOG("get_device_id: Could not get config[0] descriptor for device (err %d)\n", result);
        return result;
    }

      /* Set the configuration */
    if (desc.bNumConfigurations > 1)
    {
#ifdef DEBUG
        LOGLOG("get_device_id: More than one configuration (%d), choosing first config (%d)\n", desc.bNumConfigurations, config0->bConfigurationValue);
#endif
    }
    result = libusb_set_configuration (udev, config0->bConfigurationValue);

    libusb_free_config_descriptor (config0);

    if (result < 0)
    {
#ifdef DEBUG
        LOGLOG("get_device_id: libusb complained: %s\n", my_libusb_strerror (result));
#endif
        if (result == LIBUSB_ERROR_ACCESS)
        {
            LOGLOG("get_device_id: Make sure you run as root or set appropriate permissions\n");
        }
        else if (result == LIBUSB_ERROR_BUSY)
        {
#ifdef DEBUG
            LOGLOG("get_device_id: Maybe the kernel scanner driver or usblp claims the interface? Ignoring this error...\n");
#endif
            result = 0;
        }

    }
    return result;
}

int claimInterface(libusb_device_handle* udev ,int interface){
    int result;
    /* Check whether a kernel driver is attached to interface #0. If so, we'll
     * need to detach it.
     */
    result = libusb_kernel_driver_active(udev, interface);

    if (result == 1) { //un-mount usbhid driver
                    //HID會被Linux掛上usbhid的driver造成我們無法存取，先把它detach掉
        result = libusb_detach_kernel_driver(udev, interface);
#ifdef DEBUG
        if(!result)
            LOGLOG("get_device_id: usb_detach_kernel_driver_np : success\n");
#endif
    }
#ifdef DEBUG
    else
    {
        LOGLOG("get_device_id: Nokernel driver is active \n");
    }
#endif
    /* Claim the interface */
    result = libusb_claim_interface (udev, interface);
    if (result < 0)
    {
      LOGLOG("get_device_id: libusb complained: %s\n", my_libusb_strerror (result));
      if (result == LIBUSB_ERROR_ACCESS)
      {
          LOGLOG("get_device_id: Make sure you run as root or set appropriate permissions\n");
          return result;
      }
      else if (result == LIBUSB_ERROR_BUSY)
      {
          LOGLOG("get_device_id: Maybe the kernel scanner driver claims the scanner's interface?\n");
          return result;
       }
    }
    return result;
}

int releaseInterface(libusb_device_handle* udev ,int interface)
{
    int result = libusb_release_interface(udev, interface); //release the claimed interface
    if (result != 0) {
#ifdef DEBUG
        LOGLOG("USBWrite: Cannot Release Interface: %d \"%s\" \n", result, strerror(-result));
#endif
    }
    libusb_attach_kernel_driver(udev, interface);
    return result;
}

UsbApi::UsbApi()
    :g_interface(0)
    ,g_device(NULL)
    ,g_dev_h(NULL)
{
}

int UsbApi::getDeviceWithSerial(struct_device* pData)
{
    int ret = iterateDevices(_getUsbDeviceWithSerail ,pData);
    return ret;
}

int UsbApi::getDeviceWithSerial(int vid ,int pid ,const char* serial)
{
    struct_device data;
    memset((void*)&data ,0 ,sizeof(data));
    data.deviceInfo.vid = vid;
    data.deviceInfo.pid = pid;
    if(serial)
        strcpy(data.deviceInfo.serial ,serial);
    return getDeviceWithSerial(&data);
}

int UsbApi::init()
{
    int ret;
    ret = libusb_init(NULL);
    return ret;
}

int UsbApi::exit()
{
    libusb_exit(NULL);
    return 0;
}

int UsbApi::open(int vid, int pid, const char *serial)
{
    struct_device data;
    memset((void*)&data ,0 ,sizeof(data));
    data.deviceInfo.vid = vid;
    data.deviceInfo.pid = pid;
    if(serial)
        strcpy(data.deviceInfo.serial ,serial);
    int ret = getDeviceWithSerial(&data);
    if(ret)
        return ret;
    g_device = data.dev;
    g_dev_h = data.udev;
    ret = config(g_device ,g_dev_h);
    if(ret){
        libusb_close(g_dev_h);
        return ret;
    }
    ret = claimInterface(g_dev_h ,g_interface);
    if(ret){
        libusb_close(g_dev_h);
        return ret;
    }
    return ret;
}

int UsbApi::close()
{
    releaseInterface(g_dev_h ,g_interface);

    libusb_close(g_dev_h);
    g_dev_h = NULL;
    g_device = NULL;
}

bool UsbApi::isConnected(int vid, int pid, const char *serial)
{
    int ret = open(vid ,pid ,serial);
    if(!ret){
        close();
    }
    return !ret;
}

int UsbApi::write(char* buffer ,int bufsize)
{
    int doneByte; //for return values
    doneByte = libusb_control_transfer(g_dev_h, LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_ENDPOINT_IN |
                                       LIBUSB_RECIPIENT_INTERFACE, 0x4D, 0x3C2B, (g_interface ==1? 0x0100 : 0),
            (unsigned char *) buffer, bufsize, 5000);
//    doneByte = libusb_control_transfer(g_dev_h, 0x41, 0x4D, 0x3C2B, (g_interface ==1? 0x0100 : 0),
//            (unsigned char *) buffer, bufsize, 5000);

    if (doneByte < 0) {
        LOGLOG("USBWrite: Control write failed\n");
        *buffer = '\0';
    }
    return doneByte;

}

int UsbApi::read(char* buffer ,int bufsize)
{
    int doneByte; //for return values
    doneByte = libusb_control_transfer(g_dev_h, 0xc1, 0x00, 0x00, (g_interface ==1? 0x0100 : 0),
            (unsigned char *) buffer, bufsize, 5000);


    if (doneByte < 0) {
        LOGLOG("USBRead: Control read failed\n");
        *buffer = '\0';
    }
    return doneByte;
}

int UsbApi::getDeviceId(char *buffer, int bufsize)
{
    int
    ret = libusb_control_transfer(g_dev_h, (0x01 << 5) | 0x80 | 0x01, 0x00, 0x01,
                                  (g_interface ==1? ((1 << 8) | 0) : 0), (unsigned char *) buffer, bufsize, 5000);

    if (ret < 0)
    {
        LOGLOG("get_device_id: get 1284 fail\n");
        *buffer = '\0';
        return -1;
    }

    int length; /* Length of device ID */
    /*
     * Extract the length of the device ID string from the first two
     * bytes.  The 1284 spec says the length is stored MSB first...
     */

    length = (((unsigned) buffer[0] & 255) << 8) + ((unsigned) buffer[1] & 255);

    /*
     * Check to see if the length is larger than our buffer; first
     * assume that the vendor incorrectly implemented the 1284 spec,
     * and then limit the length to the size of our buffer...
     */

    if (length > (bufsize - 2))
        length = (((unsigned) buffer[1] & 255) << 8)
                + ((unsigned) buffer[0] & 255);

    if (length > (bufsize - 2))
        length = bufsize - 2;

    /*
     * Copy the device ID text to the beginning of the buffer and
     * nul-terminate.
     */

    memmove(buffer, buffer + 2, length);
    buffer[length] = 0;

    return 0;
}
