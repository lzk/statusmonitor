#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include "uiconfig.h"
#include <QSystemTrayIcon>

class QMenu;
class QAction;
class QDialog;
namespace Ui {
class MainWindow;
}
class QListWidgetItem;
class QCloseEvent;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
//    void closeEvent(QCloseEvent *event) override;

private slots:
    void cmdResult(int cmd,int result ,QVariant data=QVariant());

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void messageClicked();

    void on_listWidget_printers_itemDoubleClicked(QListWidgetItem *item);

    void on_btn_copy_clicked();

    void on_btn_wifi_get_clicked();

    void on_btn_get_aplist_clicked();

    void on_btn_get_ipv4_clicked();

private:
    Ui::MainWindow *ui;
    QDialog* dialog;

    QString current_printer;

    QStringList printers;
    QList<PrinterInfo_struct> printerInfos;

    void setcurrentPrinter(const QString& str);

    void updateToner(int c ,int m ,int y ,int k);

    void updateStatus(const PrinterStatus_struct& status);

    void updateOtherStatus(const QString& printer ,const PrinterStatus_struct& status);

    void updatePrinter(const QVariant& data);


    QString get_Status_string(const PrinterStatus_struct& status);

//    QAction *minimizeAction;
//    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    void createSysTray();

};

#endif // MAINWINDOW_H
