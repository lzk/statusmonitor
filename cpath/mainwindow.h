#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "statusmonitor.h"
#include <QVariant>
#include "uiconfig.h"
#include "busyrefreshlabel.h"
#include <qsystemtrayicon.h>

namespace Ui {
class MainWindow;
}
class QListWidgetItem;
class QCloseEvent;
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QString selectState;
    QString unSelectState;
    QPoint last;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    bool isPress;
    void closeEvent(QCloseEvent *e);

private slots:
    void on_Copy_clicked();

    void on_Scan_clicked();

    void on_Setting_clicked();

    void on_loginButton_clicked();


    void on_CopyImgBtn_clicked();

    void on_ScanImgBtn_clicked();

    void on_SettingImgBtn_clicked();

    void on_refreshBtn_clicked();

    void cmdResult(int cmd,int result ,QVariant data=QVariant());

    void setcurrentPrinter(const QString& str);

    void updateToner(int c ,int m ,int y ,int k);

    void updateStatus(QVariant data);

    void updateOtherStatus(const QString& printer ,const PrinterStatus_struct& status);

    void updatePrinter(const QVariant& data);

    void on_deviceNameBox_currentIndexChanged(int index);

    void set_Message_Background_Color(UIConfig::EnumStatus s);

    void onStatusCh(const PrinterStatus_struct& status);

    void errorStatus(bool bIsErrorStatus);

    void setDeviceMsg(const QString& msg, int result);

    void startScan();
    void stopScan();

    void startCycleAnimation();
    void stopCycleAnimation();

    void on_errorBtn_clicked();

    void on_btCar_clicked();

private:
    Ui::MainWindow *ui;

    QString current_printer;

//    BusyRefreshLabel *statusCycle;
    BusyRefreshLabel *cycle;

    QStringList printers;
    QList<PrinterInfo_struct> printerInfos;

    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    void createSysTray();

    bool enabledScanCopy;
    bool enabledWiFi;
};

#endif // MAINWINDOW_H
