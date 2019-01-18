#include "netio.h"
#include <QTcpSocket>
#include <QHostInfo>
#include <QUrl>
#include "api_libcups.h"
#include "log.h"
#include <QDebug>
QHostAddress get_ip_address(const QString& host)
{
    QHostAddress hostAddress;

    QHostInfo info;
    info = QHostInfo::fromName(host);
    if (!info.addresses().isEmpty()) {
        bool found = false;
        foreach (QHostAddress address, info.addresses()) {
            if(QAbstractSocket::IPv4Protocol == address.protocol()){
                hostAddress = address;
                found = true;
                break;
            }
        }
        if(!found)
            hostAddress = info.addresses().first();
//        qDebug()<<"host name:" << host << "addresses:" <<info.addresses();
    }
    return hostAddress;
}

NetIO::NetIO():
    tcpSocket(NULL)
{
    ifdelay = 0;
}
NetIO::~NetIO()
{
    if(tcpSocket)
        delete tcpSocket;
}

int NetIO::resolveUrl(const char* _url)
{
    int ret = DeviceIO::resolveUrl(_url);
    if(ret)
        return ret;
    char uri[256];
    cups_resolve_uri(_url ,uri ,sizeof(uri));
    QUrl url(uri);
    if(url.isEmpty() || url.host().isEmpty())
        url = QUrl(_url);
    QString host = url.host();//resolve_uri(url);
    LOGLOG("net io rosolve printer uri:%s" ,_url);
    LOGLOG("net io rosolved printer uri:%s" ,url.host().toLatin1().constData());
    if(host.isEmpty())
        return -1;
    hostAddress = get_ip_address(host);
    resolved_url = QString("socket://") + hostAddress.toString();
    return 0;
}

int NetIO::type()
{
    return DeviceIO::Type_net;
}

int NetIO::open(int port)
{
    if(!tcpSocket)
        tcpSocket = new QTcpSocket;
    tcpSocket->connectToHost(hostAddress, port);
    if (!tcpSocket->waitForConnected(5000)) {
//        emit error(tcp_socket.error(), tcp_socket.errorString());
//        LOG_PARA("err:%d",tcpSocket->error());
//        LOG_NOPARA("err:" + tcpSocket->errorString());
        qDebug()<<"tcp error:"<< tcpSocket->error();
        qDebug()<< tcpSocket->errorString();
        return -1;
    }
    tcpSocket->setSocketOption(QAbstractSocket::LowDelayOption ,1);
    qDebug()<<"option:"<<tcpSocket->socketOption(QAbstractSocket::LowDelayOption);
    return 0;

}

int NetIO::close(void)
{
    if(!tcpSocket)
        return -1;
//    tcpSocket->disconnectFromHost();
//    tcpSocket->waitForDisconnected();
//    tcpSocket->abort();
    tcpSocket->close();
    return 0;
}

int NetIO::write(char *buffer, int bufsize)
{
    if(!tcpSocket)
        return -1;
//    int state = tcpSocket->state();
//    LOG_PARA("tcp_socket state:%d" ,state);
    int ret;
    ret = tcpSocket->write(buffer ,bufsize);

    if(!tcpSocket->waitForBytesWritten(15000)){
        LOGLOG("tcp_socket write timeout");
        return -1;
    }
    return ret;
}

int NetIO::read(char *buffer, int bufsize)
{
    if(!tcpSocket)
        return -1;
    int bytesAvailable;
#if 0
    do{
        bytesAvailable = tcpSocket->bytesAvailable();
        if(bytesAvailable < bufsize){
//            LOG_PARA("not enough bytesAvailable:%d,buffer size:%d",bytesAvailable ,bufsize);
//            if(!tcpSocket->waitForReadyRead(10)){
            if(!tcpSocket->waitForReadyRead(15000)){
                return -1;
            }
        }else{
            break;
        }
    }
    while(1);
//    LOG_PARA("bytesAvailable:%d,buffer size:%d",bytesAvailable ,bufsize);
    return tcpSocket->read(buffer ,bufsize);
#else
    int numRead = 0, numReadTotal = 0;
    do{
        bytesAvailable = tcpSocket->bytesAvailable();
        if(bytesAvailable <= 0){
            if (!tcpSocket->waitForReadyRead(5000)){
    //            qDebug()<<"tcp error:"<< tcpSocket->error();
    //            qDebug()<< tcpSocket->errorString();
                LOGLOG("reading error:" ,tcpSocket->errorString().toUtf8().constData());
                break;
            }
            bytesAvailable = tcpSocket->bytesAvailable();
        }
        if(bytesAvailable > bufsize - numReadTotal)
            bytesAvailable = bufsize - numReadTotal;
        if(bytesAvailable > 0){
            numRead = tcpSocket->read(buffer + numReadTotal ,bytesAvailable);
            if(numRead <= 0){
                break;
            }else{
                numReadTotal += numRead;
            }
        }else{
            break;
        }
    }while(numReadTotal < bufsize);
    if(numReadTotal != bufsize)
        return -1;
    return numReadTotal;
#endif
}

int NetIO::getDeviceId(char *buffer, int bufsize)
{
    return getDeviceId_without_open(buffer ,bufsize);
}

static int _platform_net_get_device_id(const QString& device_uri,char *buffer, size_t bufsize);
int NetIO::getDeviceId_without_open(char *buffer, int bufsize)
{
    //some host name can not get device id. change to ipv4 first.
//    return snmpGetDeviceID(resolved_url.toLatin1().constData() ,buffer ,bufsize);
    return _platform_net_get_device_id(resolved_url ,buffer ,bufsize);
}

bool NetIO::isConnected()
{
    char buffer[1024];
    return !getDeviceId_without_open(buffer ,sizeof(buffer));
}

const char* NetIO::getDeviceAddress()
{
    return hostAddress.toString().toLatin1().constData();
}

int NetIO::write_bulk(char *buffer, int bufsize ,unsigned int)
{
    return write(buffer ,bufsize);
}

int NetIO::read_bulk(char *buffer, int bufsize ,unsigned int)
{
    return read(buffer ,bufsize);
}

#include <QUdpSocket>
#include <unistd.h>
static int _platform_net_get_device_id(const QString& device_uri,char *buffer, size_t bufsize)
{
    QHostAddress host_address = get_ip_address(QUrl(device_uri).host());
//    LOGLOG("get device id from host address:%s" ,host_address.toString().toLatin1().constData());

    const unsigned char data[] =
    {0x30, 0x2d, 0x02, 0x01, 00, 0x04, 0x06, 0x70, 0x75, 0x62, 0x6c, 0x69, 0x63, 0xa0, 0x20,
            0x02, 0x01, 0x04, 0x02, 0x01, 00, 0x02, 0x01, 00, 0x30, 0x15, 0x30, 0x13, 0x06,
            0x0f, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x95, 0x0b, 0x01, 0x02, 0x01, 0x02, 0x01,
     0x01, 0x03, 0x01, 0x05, 00};

    int bytes = sizeof(data);

    QUdpSocket udpSocket;
    udpSocket.connectToHost(host_address ,161);
    if (!udpSocket.waitForConnected(5000)) {
        return -1;
    }
//    udpSocket.bind(host_address ,161);
    int ret;
    ret = udpSocket.writeDatagram((const char*)data ,bytes ,host_address ,161);
    if(ret != bytes){
        return -1;
    }

    int times = 0;
    while (!udpSocket.hasPendingDatagrams()){
        times ++;
        usleep( 10 * 1000);//10ms
        if(times > 500) //5s
            break;
    }
//    qDebug()<<"wait for reading time:"<<times * 10 <<"ms";

    QByteArray ba;
    while (udpSocket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket.readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
        ba += datagram;
    }
    udpSocket.close();
    if(ba.size() > 60){
        if(bufsize >= ba.size() - 60)
            memcpy(buffer ,ba.data() + 59 ,ba.size() - 60);
    }
    return 0;
}
