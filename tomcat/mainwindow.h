#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include "uiconfig.h"
#include <QSystemTrayIcon>
#include "about.h"

class QMenu;
class QAction;
class QDialog;
namespace Ui {
class MainWindow;
}
class QTableWidgetItem;
class QCloseEvent;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
//    void closeEvent(QCloseEvent *event);// override;


private slots:
    void cmdResult(int cmd,int result ,QVariant data=QVariant());
    void about();

    void on_tabWidget_currentChanged(int index);

    void on_tableWidget_printers_itemDoubleClicked(QTableWidgetItem *item);

    void on_checkBox_clicked();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void messageClicked();
    void on_pushButton_changePassword_clicked();

    void on_checkBox_record_clicked();

    void on_btn_prepage_clicked();

    void on_btn_nextpage_clicked();

private:
    Ui::MainWindow *ui;
    About* about_dialog;

    QString current_printer;

    QStringList printers;
//    QList<PrinterInfo_struct> printerInfos;
    QList<Printer_struct> printerlist;

    void setcurrentPrinter(const QString& str);

    void updateToner(int c ,int m ,int y ,int k);

    void updateStatus(const PrinterStatus_struct& status);

    void updateOtherStatus(const QString& printer ,const PrinterStatus_struct& status);

    void updatePrinter(const QVariant& data);

    void updateJobHistory(const QVariant& data);

    QString get_Status_string(const PrinterStatus_struct& status);

    int validate_password();
//    QAction *minimizeAction;
//    QAction *maximizeAction;
//    QAction *restoreAction;
    QAction *aboutAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    void createSysTray();

    bool record_printlist;
    bool bFindOnLinePrinter = false;
    bool bCheckPrinter = true;
//    QString password;
    int jobs_page;
};

#endif // MAINWINDOW_H
