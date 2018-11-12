#include "statusmanager.h"
#include <QSettings>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/file.h>
#include "jkinterface.h"
#include "smconfig.h"

static int saveStatus(const char* printer ,PRINTER_STATUS* status)
{
    QSettings settings(filepath ,QSettings::defaultFormat());
    QString key = QString(statusKey) + printer;

    settings.beginGroup(key);
    settings.setValue("TonelStatusLevelK" ,status->TonelStatusLevelK);
    settings.setValue("TonelStatusLevelC" ,status->TonelStatusLevelC);
    settings.setValue("TonelStatusLevelM" ,status->TonelStatusLevelM);
    settings.setValue("TonelStatusLevelY" ,status->TonelStatusLevelY);

//    BYTE	TonelStatusLevelK;
//    BYTE	TonelStatusLevelC;
//    BYTE	TonelStatusLevelM;
//    BYTE	TonelStatusLevelY;

    settings.setValue("DrumStatusLifeRemain" ,status->DrumStatusLifeRemain);
    settings.setValue("CoverStatusFlags" ,status->CoverStatusFlags);
    settings.setValue("PaperTrayStatus" ,status->PaperTrayStatus);
    settings.setValue("PaperSize" ,status->PaperSize);
//    BYTE	DrumStatusLifeRemain;
//    BYTE	CoverStatusFlags;
//    BYTE	PaperTrayStatus;
//    BYTE	PaperSize;

    settings.setValue("OutputTrayLevel" ,status->OutputTrayLevel);
    settings.setValue("PrinterStatus" ,status->PrinterStatus);
    settings.setValue("OwnerName" ,status->OwnerName);
    settings.setValue("DocuName" ,status->DocuName);
//    BYTE	OutputTrayLevel;
//    BYTE	PrinterStatus;
////	wchar_t	OwnerName[16];
////	wchar_t	DocuName[16];
//    char	OwnerName[32];
//    char	DocuName[32];

    settings.setValue("ErrorCodeGroup" ,status->ErrorCodeGroup);
    settings.setValue("ErrorCodeID" ,status->ErrorCodeID);
    settings.setValue("PrintingPage" ,status->PrintingPage);
    settings.setValue("Copies" ,status->Copies);
//    BYTE	ErrorCodeGroup;
//    BYTE	ErrorCodeID;
//    WORD	PrintingPage;
//    WORD	Copies;
    settings.setValue("TotalCounter" ,status->TotalCounter);
//    settings.setValue("TRCCurve" ,status->TRCCurve);
//    settings.setValue("TonerSize" ,status->TonerSize);
//    DWORD	TotalCounter;
//    BYTE	TRCCurve[12];

//    BYTE	TonerSize[4];

    settings.setValue("PaperType" ,status->PaperType);
    settings.setValue("NonDellTonerMode" ,status->NonDellTonerMode);
    settings.setValue("AioStatus" ,status->AioStatus);
//    BYTE	PaperType;
//    BYTE	NonDellTonerMode;
//    BYTE	AioStatus;
//    //BYTE	bReserved;

    settings.setValue("bPhyPrinterStatus" ,status->bPhyPrinterStatus);
    settings.setValue("LowTonerAlert" ,status->LowTonerAlert);
//    BYTE	bPhyPrinterStatus;
//    //WORD	wReserved1;
//    BYTE	LowTonerAlert;
//    BYTE	bReserved1;
//    WORD	wReserved2;
    settings.endGroup();
    settings.sync();
    return 0;
}

static int getStatus(const char* printer ,PRINTER_STATUS* status)
{
    QSettings settings(filepath ,QSettings::defaultFormat());
    QString key = QString(statusKey) + printer;
    if(!settings.allKeys().contains(key + "/PrinterStatus")){
        return -1;
    }
    settings.beginGroup(key);
    status->TonelStatusLevelK = settings.value("TonelStatusLevelK").toInt();
    status->TonelStatusLevelC = settings.value("TonelStatusLevelC").toInt();
    status->TonelStatusLevelM = settings.value("TonelStatusLevelM").toInt();
    status->TonelStatusLevelY = settings.value("TonelStatusLevelY").toInt();

    status->DrumStatusLifeRemain = settings.value("DrumStatusLifeRemain").toInt();
    status->CoverStatusFlags = settings.value("CoverStatusFlags").toInt();
    status->PaperTrayStatus = settings.value("PaperTrayStatus").toInt();
    status->PaperSize = settings.value("PaperSize").toInt();

    status->OutputTrayLevel = settings.value("OutputTrayLevel").toInt();
    status->PrinterStatus = settings.value("PrinterStatus").toInt();
    strncpy(status->OwnerName ,settings.value("OwnerName").toString().toUtf8().constData() ,sizeof(status->OwnerName));
    strncpy(status->DocuName ,settings.value("DocuName").toString().toUtf8().constData() ,sizeof(status->DocuName));

    status->ErrorCodeGroup = settings.value("ErrorCodeGroup").toInt();
    status->ErrorCodeID = settings.value("ErrorCodeID").toInt();
    status->PrintingPage = settings.value("PrintingPage").toInt();
    status->Copies = settings.value("Copies").toInt();

    status->TotalCounter = settings.value("TotalCounter").toInt();
//    status->TRCCurve = settings.value("TRCCurve").toInt();
//    status->TonerSize = settings.value("TonerSize").toInt();

    status->PaperType = settings.value("PaperType").toInt();
    status->NonDellTonerMode = settings.value("NonDellTonerMode").toInt();
    status->AioStatus = settings.value("AioStatus").toInt();

    status->bPhyPrinterStatus = settings.value("bPhyPrinterStatus").toInt();
    status->LowTonerAlert = settings.value("LowTonerAlert").toInt();

    settings.endGroup();
    return 0;
}

static int clear()
{
    QSettings settings(filepath ,QSettings::defaultFormat());
    settings.clear();
    settings.sync();
    return 0;
}

static int clearPrinters()
{
    QSettings settings(filepath ,QSettings::defaultFormat());
    QString key = QString(printersKey);
    settings.remove(key);
    settings.sync();
    return 0;
}

//static int savePrinter(const char* printer)
//{
//    QSettings settings(filepath ,QSettings::defaultFormat());
//    QString key = QString(printersKey) + printer;
//    QVariant value(1);
//    settings.setValue(key ,value);
//    settings.sync();
//    return 0;
//}
//static QStringList getPrinter()
//{
//    QSettings settings(filepath ,QSettings::defaultFormat());
//    QString key = QString(printersKey);
//    settings.beginGroup(key);
//    return settings.childKeys();
////    return settings.childGroups();
//}

static int savePrinters(QStringList printers)
{
    QSettings settings(filepath ,QSettings::defaultFormat());
    QString key = QString(printersKey);
    QVariant value(printers);
    settings.setValue(key ,value);
    settings.sync();
    return 0;
}

static QStringList getPrinters()
{
    QSettings settings(filepath ,QSettings::defaultFormat());
    QString key = QString(printersKey);
    QVariant value;
    value = settings.value(key);
    if(value != QVariant()){
        return value.toStringList();
    }
    return QStringList();
}

StatusManager::StatusManager():
    fp(NULL)
{
}

int StatusManager::lock(const char* filename)
{
    int ret = -1;
    fp = fopen(filename, "ab+");
    int fd;
    if(fp){
#ifdef Q_OS_MAC
        fd = fp->_file;
#else
        fd = fp->_fileno;
#endif
        ret = flock(fd, LOCK_EX);
        if (ret){
            fclose(fp);
            fp = NULL;
        }
    }
    return ret;
}

int StatusManager::trylock(const char* filename)
{
    int ret = -1;
    fp = fopen(filename, "ab+");
    int fd;
    if(fp){
#ifdef Q_OS_MAC
        fd = fp->_file;
#else
        fd = fp->_fileno;
#endif
        if (flock(fd, LOCK_EX | LOCK_NB)){
            ret = 0;
        }else{
            fclose(fp);
            fp = NULL;
        }
    }
    return ret;
}

int StatusManager::unlock()
{
    if(fp){
        int fd;
#ifdef Q_OS_MAC
        fd = fp->_file;
#else
        fd = fp->_fileno;
#endif
        flock(fd, LOCK_UN);
        fclose(fp);
        fp = NULL;
    }
    return 0;
}

int StatusManager::saveStatusToFile(const char* printer ,PRINTER_STATUS* status)
{
    int ret;
    ret = lock(lockfile);
    if(!ret){
        ret = saveStatus(printer ,status);
        unlock();
    }
    return ret;
}

int StatusManager::getStatusFromFile(const char* printer ,PRINTER_STATUS* status)
{
    int ret;
    ret = lock(lockfile);
    if(!ret){
        ret = getStatus(printer ,status);
        unlock();
    }
    return ret;
}

int StatusManager::clearFile()
{
    int ret;
    ret = lock(lockfile);
    if(!ret){
        ret = clear();
        unlock();
    }
    return ret;
}

int StatusManager::clearPrintersOfFile()
{
    int ret;
    ret = lock(lockfile);
    if(!ret){
        ret = clearPrinters();
        unlock();
    }
    return ret;
}

QStringList StatusManager::getPrintersFromFile()
{
    QStringList printers;
    int ret;
    ret = lock(lockfile);
    if(!ret){
        printers = getPrinters();
        unlock();
    }
    return printers;
}

int StatusManager::savePrintersToFile(QStringList printers)
{
    int ret;
    ret = lock(lockfile);
    if(!ret){
        ret = savePrinters(printers);
        unlock();
    }
    return ret;
}
