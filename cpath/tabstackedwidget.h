#ifndef TABSTACKEDWIDGET_H
#define TABSTACKEDWIDGET_H

#include <QStackedWidget>
#include "settingsstackedwidget.h"
#include "moresettingsforscan.h"
#include "moresettingsforcopy.h"
#include "uinterface.h"

namespace Ui {
class TabStackedWidget;
}

class TabStackedWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit TabStackedWidget(QWidget *parent = 0);
    ~TabStackedWidget();

   // SettingsStackedWidget *settingsStackedWidget;

public slots:
    void on_btn_WiFi_clicked();
    bool getScrollAreaImageStatus();
    void set_copy_enabled(bool enabled);
    void set_scan_enabled(bool enabled);
    void set_setting_enabled(bool enabled);

private slots:
    void on_cBox_IsIDCard_clicked(bool checked);

    void on_cBox_DuplexCopy_clicked(bool checked);

    void on_btn_SoftAP_clicked();

    void on_btn_TCPIPV4_clicked();

    void on_btn_TCPIPV6_clicked();

    void on_btn_PowerSave_clicked();

    void on_btn_UserConfig_clicked();

    void on_btn_Password_clicked();

    void on_btn_Scan_clicked();

    void on_scrollArea_ScanImage_itemSelectionChanged();

    void on_btn_MoreSetting_Scan_clicked();

    void setDefault_Copy();

    void on_btn_MoreSetting_Copy_clicked();

    void on_btn_DensityAdd_clicked();

    void on_btn_DensityReduce_clicked();

    void on_btn_CopyNumAdd_clicked();

    void on_btn_CopyNumReduce_clicked();

    void startCycleEmit();
    void stopCycleEmit();

    void on_btn_Copy_clicked();

    void cmdResult(int,int,QVariant);

private:
    Ui::TabStackedWidget *ui;

    QString selectState;
    QString selectHover;
    QString selectPressed;

    QString unSelectState;
    QString unSelectHover;
    QString unSelectPressed;

    int value_Density;

    QStringList filenamelist;

    Param_Copy paramCopy;

    Param_Scan paramScan;

signals:
    void cycleStartFromTab();
    void cycleStopFromTab();
};

#endif // TABSTACKEDWIDGET_H
