#ifndef COMMONAPI_H
#define COMMONAPI_H
#include <QString>
#include <QVariant>
#include "serverthread.h"
#include "devicemanager.h"
#include "usbio.h"
#include "netio.h"

bool appSettings(const QString& key ,QVariant& value ,const QVariant& defaultValue ,bool set = false);

bool isRunning(const char* server_path);
#endif // COMMON_H
