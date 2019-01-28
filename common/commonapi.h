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
QString get_string_from_shell_cmd(const QString& cmd ,int mode = 0);
#endif // COMMON_H
