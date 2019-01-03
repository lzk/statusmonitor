#ifndef WLANTITLECELL_H
#define WLANTITLECELL_H

#include <QStackedWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QObjectList>
#include "typedefine.h"
#include <QTimer>
#include <QLabel>
#include <QList>
#include "busyrefreshlabel.h"
#include "typedefine.h"
#include "uinterface.h"
#include "qvariant.h"

namespace Ui {
class WlanTitleCell;
}

class WlanTitleCell : public QStackedWidget
{
    Q_OBJECT

public:
    explicit WlanTitleCell(QWidget *parent = 0, bool wlanON=true, bool *_islogin = 0);
    ~WlanTitleCell();

//    BusyRefreshLabel *cycle;
    APInfo *currentAp;

public slots:
    void updateAP();
    void on_btFlesh_clicked();

private slots:
    void on_btManualWiFi_clicked();
    void on_btCancel_clicked();
    void on_btWLANON1_clicked();
    void on_btWLANON2_clicked();

    void getSizeChanged(QSize, QSize);
    void addCell(QString ssid, EncrypType type, APInfo info, bool isConnected);
    void initCell(cmdst_wifi_get wifi_para, cmdst_aplist_get aplist);
    void on_timeout();

    void getConnectAction(QWidget*);
    void getConnectResult(QWidget*, bool);

    void on_combox_encryption_currentIndexChanged(int index);

    void on_btConnect_clicked();

    void on_btKey1_toggled(bool checked);

    void on_btKey2_toggled(bool checked);

    void on_btKey3_toggled(bool checked);

    void on_btKey4_toggled(bool checked);

    int checkSSID(QString SSID);

    void on_checkBox_visiable_toggled(bool checked);

    void cmdResult(int,int,QVariant);

signals:
    void statusChange();
    void tryToConnect(APInfo);

private:
    Ui::WlanTitleCell *ui;

    QWidget *widget;
    QSize currentSize;
    QVBoxLayout pageLayout;

    QList<QWidget *> apList;


    QTimer *timer1;


    QList<APInfo> aList;

    bool isWlanOn;
    bool isManual;

    int cycleCount;
    int currentAPID;

    cmdst_wifi_get wifi_para;
    cmdst_wifi_get orin_wifi_para;
    bool *islogin;
    bool isWitch;
    bool is_wifi_now_on;
    bool isDoingCMD;
    int times;

signals:
    void cycleStartFromWT();
    void cycleStopFromWT();
};

#endif // WLANTITLECELL_H
