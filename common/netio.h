#ifndef NETIO_H
#define NETIO_H

#include <QHostAddress>
#include "deviceio.h"
class QTcpSocket;
class NetIO :public DeviceIO
{
public:
    NetIO();
    ~NetIO();

    virtual int type();
    virtual int open(int port = 9100);
//    virtual int open(const char* url ,int port = 0);
    virtual int close(void);
    virtual int write(char *buffer, int bufsize);
    virtual int read(char *buffer, int bufsize);
    virtual int resolveUrl(const char* url);
    virtual bool isConnected();
    virtual int getDeviceId(char *buffer, int bufsize);
    virtual const char* getDeviceAddress();

private:
    QTcpSocket* tcpSocket;
    QHostAddress hostAddress;
};

#endif // NETIO_H
