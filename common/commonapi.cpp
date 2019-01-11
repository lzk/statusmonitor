#include <QSettings>
#include "log.h"
#include "trans.h"
bool appSettings(const QString& key ,QVariant& value ,const QVariant& defaultValue ,bool set)
{
    bool result = true;
    QSettings settings;
    settings.setDefaultFormat(QSettings::NativeFormat);
    if(set){
        settings.setValue(key ,value);
        LOGLOG("set key %s value:%s" ,key.toLatin1().constData() ,settings.value(key ,defaultValue).toString().toLatin1().constData());
    }else{
        value = settings.value(key ,defaultValue);
        LOGLOG("get key %s value:%s" ,key.toLatin1().constData() ,value.toString().toLatin1().constData());
    }
    return result;
}
#if 1
#include <QApplication>
#include <QLocalSocket>
 #include <QLocalServer>
#include <QFile>
QLocalServer* m_localServer;
bool isRunning(const char* serverName)
{
    bool running = true;
    QLocalSocket socket;
    socket.connectToServer(serverName);
    if (!socket.waitForConnected()) {
//        LOGLOG(socket.errorString());
        if(QFile::exists(serverName))
            QFile::remove(serverName);

        m_localServer = new QLocalServer(qApp);
        if (!m_localServer->listen(serverName)) {
            if (m_localServer->serverError() == QAbstractSocket::AddressInUseError
                    && QFile::exists(serverName)) { //make sure listening success
                QFile::remove(serverName);
                m_localServer->listen(serverName);
            }
        }
        if(!system((QString("chmod a+w ") + serverName+ " 2>/dev/null").toLatin1())){
            ;
        }
        running = false;
    }
    return running;
}
#else
bool isRunning(const char* server_path)
{
    bool running = true;
    Trans_Client tc(server_path);
    int ret = tc.tryConnectToServer();
    switch (ret) {
    case 0:
        break;
    case -2:
        running = false;
        break;
    default:
        LOGLOG("There is something error!");
        break;
    }
    return running;
}
#endif
