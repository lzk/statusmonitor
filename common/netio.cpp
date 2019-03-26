#include "netio.h"
#include <QTcpSocket>
#include <QHostInfo>
#include <QUrl>
#include "api_libcups.h"
#include "log.h"

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
        if(!found){
            foreach (QHostAddress address, info.addresses()) {
                if(QAbstractSocket::IPv6Protocol == address.protocol()){
                    hostAddress = address;
                    found = true;
                    break;
                }
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

int NetIO::resolveUrl(const char* url)
{
    if(!url)
        return -1;
    QString str_url(url);
    if((device_uri[0] != 0) && !str_url.compare(device_uri)){
        return 0;
    }
    int ret = DeviceIO::resolveUrl(url);
    if(ret)
        return ret;
    QUrl real_url;
    if(str_url.startsWith("dnssd://")){
        char uri[256];
        cups_resolve_uri(url ,uri ,sizeof(uri));
        real_url = QUrl(uri);
    }
    if(real_url.isEmpty() || real_url.host().isEmpty())
        real_url = QUrl(url);
    QString host = real_url.host();//resolve_uri(url);
//    LOGLOG("net io rosolve printer uri:%s" ,_url);
//    LOGLOG("net io rosolved printer uri:%s" ,url.host().toLatin1().constData());
    if(host.isEmpty())
        return -1;
    hostAddress = get_ip_address(host);
    if(hostAddress.protocol() == QAbstractSocket::IPv6Protocol){
        resolved_url = QString("socket://[") + hostAddress.toString() +"]";
    }else{
        resolved_url = QString("socket://") + hostAddress.toString();
    }
    return 0;
}

int NetIO::type()
{
    return DeviceIO::Type_net;
}

int NetIO::open(int port)
{
    if(device_is_open){
        LOGLOG("device is opened");
        return -1;
    }
    if(!tcpSocket)
        tcpSocket = new QTcpSocket;
    tcpSocket->connectToHost(hostAddress, port);
    if (!tcpSocket->waitForConnected(5000)) {
        LOGLOG("tcpsocket error code:%d",tcpSocket->error());
        LOGLOG("tcpsocket error:%s" ,tcpSocket->errorString().toLatin1().constData());
        tcpSocket->close();
        return -1;
    }
    device_is_open = true;
//    tcpSocket->setSocketOption(QAbstractSocket::LowDelayOption ,1);
//    qDebug()<<"option:"<<tcpSocket->socketOption(QAbstractSocket::LowDelayOption);
    return 0;

}

int NetIO::close(void)
{
    if(!tcpSocket)
        return -1;
//    tcpSocket->disconnectFromHost();
//    tcpSocket->waitForDisconnected();
//    tcpSocket->abort();
    if(device_is_open){
        device_is_open = false;
        tcpSocket->close();
    }
    return 0;
}

int NetIO::write(char *buffer, int bufsize)
{
    if(!device_is_open){
        return -1;
    }
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
    if(!device_is_open){
        return -1;
    }
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
 //       if (!tcpSocket->waitForReadyRead(15000)){
 //           LOGLOG("%s" ,tcpSocket->errorString().toUtf8().constData());
 //           break;
 //       }
        bytesAvailable = tcpSocket->bytesAvailable();
        if(bytesAvailable <= 0){
            if (!tcpSocket->waitForReadyRead(30000)){
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

static int _platform_net_get_device_id(const QString& device_uri,char *buffer, int bufsize);
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

#if 1
//can not work well
#include <QUdpSocket>
#include <unistd.h>
static int _platform_net_get_device_id(const QString& device_uri,char *buffer, int bufsize)
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
    int ret;
    ret = udpSocket.write((const char*)data ,bytes);
//    ret = udpSocket.writeDatagram((const char*)data ,bytes ,host_address ,161);
    if(ret != bytes){
        LOGLOG("udp write fial!");
        return -1;
    }
//    bool bind_ok = udpSocket.bind(host_address ,161);
//    if(!bind_ok){
//        LOGLOG("udp bind fail!");
////        return -2;
//    }
    int times = 0;
    while (!udpSocket.hasPendingDatagrams()){
        times ++;
        usleep( 10 * 1000);//10ms
        if(times > 500){//5s
            LOGLOG("udp no data to read!");
            break;
        }
    }

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

    ret = -1;
    if(ba.size() > 60){
        if(bufsize >= ba.size() - 60){
            ret = 0;
            memcpy(buffer ,ba.data() + 59 ,ba.size() - 60);
        }
    }
    return ret;
}

#else
//not complete!!!
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
//enterprises.2699.1.2.1.2.1.1.3.1
//enterprises.26266.86.10.1.1.1.1.0
static const oid oidName[] = {1,3,6,1,4,1,2699,1,2,1,2,1,1,3,1};
static const size_t sizeofOidName = sizeof(oidName)/sizeof(oidName[0]);
bool snmpGetResponse(char* ip)
{
    bool ret = true;
    netsnmp_session session;
    init_snmp ("snmp");
    snmp_sess_init (&session);
    session.version = SNMP_VERSION_2c;
    session.community = (u_char*)"public";
    session.community_len = strlen ((const char*)session.community);
    session.peername = ip;//被监控主机的IP地址
    session.timeout = 5 *1000 * 1000;
    netsnmp_session *ss;
    netsnmp_pdu *pdu;
    ss = snmp_open (&session);
    if (ss == NULL){
        snmp_sess_perror ("snmp_open", &session);
        return (1);
    }

    int new_length=sizeofOidName;
    pdu = snmp_pdu_create (SNMP_MSG_GET);
    snmp_add_null_var (pdu, oidName, new_length);

    netsnmp_pdu *response;
    int status = snmp_synch_response (ss, pdu, &response);
    if (status != STAT_SUCCESS || !response){
        snmp_sess_perror ("snmp_synch_response", ss);
        ret = false;
    }else{
        if (response->errstat != SNMP_ERR_NOERROR){
            fprintf (stderr, "snmp: Error in packet: %s\n",snmp_errstring (response->errstat));
            ret = false;
        }
        snmp_free_pdu (response);
    }
    snmp_close (ss);
    return ret;
}

static int _platform_net_get_device_id(const QString& device_uri,char *buffer, int bufsize)
{
    QHostAddress host_address = get_ip_address(QUrl(device_uri).host());
    QString address = host_address.toString();
//    snmpGet
}
#endif
