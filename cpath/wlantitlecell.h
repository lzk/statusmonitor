#ifndef WLANTITLECELL_H
#define WLANTITLECELL_H

#include <QStackedWidget>
#include "busyrefreshlabel.h"
#include <QVBoxLayout>
#include "typedefine.h"

namespace Ui {
class WlanTitleCell;
}

class WlanTitleCell : public QStackedWidget
{
    Q_OBJECT

public:
    explicit WlanTitleCell(QWidget *parent,  bool wlanON, bool *_islogin);
    ~WlanTitleCell();

    BusyRefreshLabel *cycle;
    APInfo *currentAp;

public slots:
    void updateAP();
    void on_btFlesh_clicked();

private slots:
    void on_btManualWiFi_clicked();
    void on_btCancel_clicked();

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

    bool *islogin;
    bool isWitch;
    bool is_wifi_now_on;
    bool isDoingCMD;
    int times;
};

#endif // WLANTITLECELL_H
