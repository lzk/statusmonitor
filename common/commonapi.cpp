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

bool isRunning(const char* server_path)
{
    bool running = true;
    Trans_Client tc(server_path);
    int ret = tc.tryConnectToServer();
    switch (ret) {
    case 0:
        LOGLOG("There has been a same app running!");
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
