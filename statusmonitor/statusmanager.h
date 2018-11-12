#ifndef STATUSMANAGER_H
#define STATUSMANAGER_H

#include <stdio.h>
#include <QStringList>
#include "status.h"
class StatusManager
{
public:
    StatusManager();

    int saveStatusToFile(const char* printer ,PRINTER_STATUS* status);
    int getStatusFromFile(const char* printer ,PRINTER_STATUS* status);
    int clearFile();
    QStringList getPrintersFromFile();
    int clearPrintersOfFile();
    int savePrintersToFile(QStringList printers);

private:
    int lock(const char* path);
    int trylock(const char* path);
    int unlock();
    FILE* fp;
};

#endif // STATUSMANAGER_H
