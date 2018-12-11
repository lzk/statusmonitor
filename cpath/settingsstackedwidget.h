#ifndef SETTINGSSTACKEDWIDGET_H
#define SETTINGSSTACKEDWIDGET_H

#include <QStackedWidget>
#include "wlantitlecell.h"
#include <qscrollarea.h>
#include <qtimer.h>

namespace Ui {
class SettingsStackedWidget;
}

class SettingsStackedWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit SettingsStackedWidget(QWidget *parent = 0);
    ~SettingsStackedWidget();

    WlanTitleCell *titelCell;
    BusyRefreshLabel *cycle;
    void changeStackIndex(int index);

private:
    Ui::SettingsStackedWidget *ui;

    QScrollArea *scrollArea;

    QTimer *timer1;
    bool isAPOpen;
    bool isWLANON;
    bool isLogn;

    bool isLowHumidity;
    bool isConrtolM;
    bool isCoolingM;

    bool isADD;
    int timercounter;
    bool isDoingCMD;
    int retryTimes;

private slots:
    void initAP();
    void initIP();
    void initPowerSave();
    void initPassword();
    //void initAdvanceSetting(cmdst_userconfig config, bool isFiest);
};

#endif // SETTINGSSTACKEDWIDGET_H
