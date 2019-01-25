#include "tabstackedwidget.h"
#include "ui_tabstackedwidget.h"
#include "qdebug.h"
#include "mainwindow.h"
#include "animationdlg.h"
#include <QGraphicsDropShadowEffect>
#include <QValidator>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QPainter>
#include <QTime>
#include "settingwarming.h"
#include <qdesktopservices.h>
#include <qurl.h>
#include <qapplication.h>
#include "scannerapp.h"

TabStackedWidget::TabStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::TabStackedWidget)
{
    ui->setupUi(this);

    //init copyNum,defaule value is 1;
//    QRegExp rx("^([0-9]|[1-9]\\d)$");
//    QRegExpValidator *pReg = new QRegExpValidator(rx,this);
//    ui->copyNum->setValidator(pReg);
    ui->copyNum->setText(tr("1"));

    //init Density,default is level2;
    value_Density = 2;
    ui->mark0_Density->setStyleSheet("background-color: rgb(53, 177, 20);");
    ui->mark1_Density->setStyleSheet("background-color: rgb(53, 177, 20);");
    ui->mark2_Density->setStyleSheet("background-color: rgb(154, 238, 117);");
    ui->mark3_Density->setStyleSheet("background-color: rgb(154, 238, 117);");
    ui->mark4_Density->setStyleSheet("background-color: rgb(154, 238, 117);");

    //init Param_Copy
    this->setDefault_Copy();

    selectState = "QPushButton{border-image: url(:/Images/Btn_Green.png);color:white;}";
    selectHover = "QPushButton::hover{color:black;}";
    selectPressed = "QPushButton::pressed{border-image: url(:/Images/Btn_Green_Pressed.png);color:white;}";

    unSelectState = "QPushButton{border-image: url(:/Images/Btn_Gray.png);}";
    unSelectHover = "QPushButton::hover{color: rgb(52, 212, 34);}";
    unSelectPressed = "QPushButton::pressed{border-image: url(:/Images/Btn_Gray_Pressed.png);color:white;}";

    on_scrollArea_ScanImage_itemSelectionChanged();

    paramScan.scan_doctype = T_Photo;
    paramScan.scan_dpi = Scan_300DPI;
    paramScan.colorModel = Color;
    paramScan.contrast = 50;
    paramScan.brightness = 50;
    paramScan.scan_size = Scan_A4;
    paramScan.scan_type = Hight_Speed;

    QString labelTitle = tr("ResStr_Scanned_image_size");
    QString labelText = QString("%1%2").arg(labelTitle).arg("24.89MB");
    ui->label_ImageSize->setText(labelText);

    connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)), this ,SLOT(cmdResult(int,int,QVariant)));

    connect(ui->settingStackedWidget,SIGNAL(cycleStart()),this,SLOT(startCycleEmit()));
    connect(ui->settingStackedWidget,SIGNAL(cycleStop()),this,SLOT(stopCycleEmit()));
    connect(ui->settingStackedWidget->titelCell,SIGNAL(cycleStartFromWT()),this,SLOT(startCycleEmit()));
    connect(ui->settingStackedWidget->titelCell,SIGNAL(cycleStopFromWT()),this,SLOT(stopCycleEmit()));
    connect(gUInterface,SIGNAL(signal_update_scan_progress(int)),this,SLOT(updateScanProcess(int)));
}

void TabStackedWidget::cmdResult(int cmd,int result,QVariant data)
{
    switch(cmd)
    {
    case UIConfig::LS_CMD_COPY:

        break;
    case UIConfig::CMD_Scan:
    {

        if(!result)
        {
            ScanSettings scanSettings = data.value<ScanSettings>();

            QString image_path = scanSettings.filename;
            qDebug()<<scanSettings.calc_data.target.pixels_per_line<<"x"<<scanSettings.calc_data.target.total_lines;
            qDebug()<<scanSettings.filename;
            QSize size = QSize(scanSettings.calc_data.target.pixels_per_line,scanSettings.calc_data.target.total_lines);
            ui->scrollArea_ScanImage->add_image_item(image_path ,size);
        }
//        else if(result == STATUS_Cancel)
//        {
            //scan cancel
//        }
        gUInterface->emitStopScanSignal();
        ui->scrollArea_ScanImage->setEnabled(true);
        if(ui->scrollArea_ScanImage->selectedItems().isEmpty()){
            ui->btn_ScanSave->setEnabled(false);
        }else{
            ui->btn_ScanSave->setEnabled(true);
        }
        ui->btn_MoreSetting_Scan->setEnabled(true);
        ui->btn_Scan->setEnabled(true);
        ui->btn_ScanCancel->setEnabled(false);
        ui->progressBar_Scan->setValue(0);
    }
        break;
    default:
        break;
    }
}

void TabStackedWidget::updateScanProcess(int progress)
{
//    qDebug()<<"TabStackedWidget:"<<progress;
    ui->progressBar_Scan->setValue(progress);
}

bool TabStackedWidget::getScrollAreaImageStatus()
{

    if(ui->scrollArea_ScanImage->count() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void TabStackedWidget::setDefault_Copy()
{
    copycmdset para;
    copycmdset *p = &para;
    memcpy(p ,&default_copy_parameter ,sizeof(default_copy_parameter));

    paramCopy.scaling = (int)p->scale;
    paramCopy.docType = (DocType_Copy)p->scanMode;
    paramCopy.docSize = (DocSize_Copy)p->orgSize;
    paramCopy.docDpi = (DocDpi_Copy)p->dpi;
    paramCopy.outputSize = (OutPutSize_Copy)p->paperSize;
    paramCopy.paperType = (MediaType_Copy)p->mediaType;
    paramCopy.isMultiPage = false;
    paramCopy.multiMode = (MultiMode_Copy)p->nUp;
    paramCopy.promptInfo.isIDCard = true;
    paramCopy.promptInfo.isMultible = true;

}

void TabStackedWidget::on_scrollArea_ScanImage_itemSelectionChanged()
{
    if(ui->scrollArea_ScanImage->selectedItems().isEmpty()){
        ui->btn_ScanSave->setEnabled(false);
    }else{
        ui->btn_ScanSave->setEnabled(true);
    }
}

TabStackedWidget::~TabStackedWidget()
{
    delete ui;
}

void TabStackedWidget::on_btn_WiFi_clicked()
{
    ui->img_WiFi->setStyleSheet("border-image: url(:/Images/Wireless_Active.png)");
    ui->img_SoftAP->setStyleSheet("border-image: url(:/Images/SoftAP.png)");
    ui->img_TCPIPV4->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_TCPIPV6->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password.png)");

    ui->btn_WiFi->setStyleSheet(selectState+selectHover+selectPressed);
    ui->btn_SoftAP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIPV4->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIPV6->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_PowerSave->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_UserConfig->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_Password->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);

    //neil add for settings page 2016-01-29
    ui->settingStackedWidget->changeStackIndex(0);
    ui->settingStackedWidget->setCurrentIndex(0);
}

void TabStackedWidget::on_btn_SoftAP_clicked()
{
    ui->img_WiFi->setStyleSheet("border-image: url(:/Images/Wireless.png)");
    ui->img_SoftAP->setStyleSheet("border-image: url(:/Images/SoftAP_Active.png)");
    ui->img_TCPIPV4->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_TCPIPV6->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password.png)");

    ui->btn_WiFi->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_SoftAP->setStyleSheet(selectState+selectHover+selectPressed);
    ui->btn_TCPIPV4->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIPV6->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_PowerSave->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_UserConfig->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_Password->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);

    //neil add for settings page 2016-01-29
   ui->settingStackedWidget->changeStackIndex(1);
    ui->settingStackedWidget->setCurrentIndex(1);
}

void TabStackedWidget::on_btn_TCPIPV4_clicked()
{
    ui->img_WiFi->setStyleSheet("border-image: url(:/Images/Wireless.png)");
    ui->img_SoftAP->setStyleSheet("border-image: url(:/Images/SoftAP.png)");
    ui->img_TCPIPV4->setStyleSheet("border-image: url(:/Images/TCPIP_Active.png)");
    ui->img_TCPIPV6->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password.png)");

    ui->btn_WiFi->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_SoftAP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIPV4->setStyleSheet(selectState+selectHover+selectPressed);
    ui->btn_TCPIPV6->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_PowerSave->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_UserConfig->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_Password->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);

    //neil add for settings page 2016-01-29
    ui->settingStackedWidget->changeStackIndex(2);
    ui->settingStackedWidget->setCurrentIndex(2);
}

void TabStackedWidget::on_btn_TCPIPV6_clicked()
{
    ui->img_WiFi->setStyleSheet("border-image: url(:/Images/Wireless.png)");
    ui->img_SoftAP->setStyleSheet("border-image: url(:/Images/SoftAP.png)");
    ui->img_TCPIPV4->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_TCPIPV6->setStyleSheet("border-image: url(:/Images/TCPIP_Active.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password.png)");

    ui->btn_WiFi->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_SoftAP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIPV4->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIPV6->setStyleSheet(selectState+selectHover+selectPressed);
    ui->btn_PowerSave->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_UserConfig->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_Password->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);

    //neil add for settings page 2016-01-29
    ui->settingStackedWidget->changeStackIndex(3);
    ui->settingStackedWidget->setCurrentIndex(3);
}

void TabStackedWidget::on_btn_PowerSave_clicked()
{
    ui->img_WiFi->setStyleSheet("border-image: url(:/Images/Wireless.png)");
    ui->img_SoftAP->setStyleSheet("border-image: url(:/Images/SoftAP.png)");
    ui->img_TCPIPV4->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_TCPIPV6->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave_Active.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password.png)");

    ui->btn_WiFi->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_SoftAP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIPV4->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIPV6->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_PowerSave->setStyleSheet(selectState+selectHover+selectPressed);
    ui->btn_UserConfig->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_Password->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);

    //neil add for settings page 2016-01-29
    ui->settingStackedWidget->changeStackIndex(4);
    ui->settingStackedWidget->setCurrentIndex(4);
}

void TabStackedWidget::on_btn_UserConfig_clicked()
{
    ui->img_WiFi->setStyleSheet("border-image: url(:/Images/Wireless.png)");
    ui->img_SoftAP->setStyleSheet("border-image: url(:/Images/SoftAP.png)");
    ui->img_TCPIPV4->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_TCPIPV6->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig_Active.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password.png)");

    ui->btn_WiFi->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_SoftAP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIPV4->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIPV6->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_PowerSave->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_UserConfig->setStyleSheet(selectState+selectHover+selectPressed);
    ui->btn_Password->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);

    //neil add for settings page 2016-01-29
    ui->settingStackedWidget->changeStackIndex(5);
    ui->settingStackedWidget->setCurrentIndex(5);
}

void TabStackedWidget::on_btn_Password_clicked()
{
    ui->img_WiFi->setStyleSheet("border-image: url(:/Images/Wireless.png)");
    ui->img_SoftAP->setStyleSheet("border-image: url(:/Images/SoftAP.png)");
    ui->img_TCPIPV4->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_TCPIPV6->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password_Active.png)");

    ui->btn_WiFi->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_SoftAP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIPV4->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIPV6->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_PowerSave->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_UserConfig->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_Password->setStyleSheet(selectState+selectHover+selectPressed);

    //neil add for settings page 2016-01-29
    ui->settingStackedWidget->changeStackIndex(6);
    ui->settingStackedWidget->setCurrentIndex(6);
}


void TabStackedWidget::on_cBox_IsIDCard_clicked(bool checked)
{
    if (true==checked)
    {
        ui->cBox_IsIDCard->setStyleSheet("border-image: url(:/Images/CheckBox_Open.png);");
        ui->icon_IDCardCopy->setStyleSheet("border-image: url(:/Images/IDCardCopyIconEnable.png);");
        ui->btn_Copy->setText(tr("ResStr_ID_Card_Copy"));

        ui->cBox_DuplexCopy->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
        ui->cBox_DuplexCopy->setChecked(false);
        ui->icon_DuplexCopy->setStyleSheet("border-image: url(:/Images/DulplexCopyIconDisable.tif);");
    }
    else
    {
        ui->cBox_IsIDCard->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
        ui->icon_IDCardCopy->setStyleSheet("border-image: url(:/Images/IDCardCopyIconDisable.png);");
        ui->btn_Copy->setText(tr("ResStr_ExtraAdd_Copy"));
    }
}

void TabStackedWidget::on_cBox_DuplexCopy_clicked(bool checked)
{
    if (true==checked)
    {
        ui->cBox_IsIDCard->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
        ui->cBox_IsIDCard->setChecked(false);
        ui->icon_IDCardCopy->setStyleSheet("border-image: url(:/Images/IDCardCopyIconDisable.png);");

        ui->cBox_DuplexCopy->setStyleSheet("border-image: url(:/Images/CheckBox_Open.png);");
        ui->icon_DuplexCopy->setStyleSheet("border-image: url(:/Images/DulplexCopyIconEnable.tif);");
        ui->btn_Copy->setText(tr("ResStr_Duplex_Copy"));
    }
    else
    {
        ui->cBox_DuplexCopy->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
        ui->icon_DuplexCopy->setStyleSheet("border-image: url(:/Images/DulplexCopyIconDisable.tif);");
        ui->btn_Copy->setText(tr("ResStr_ExtraAdd_Copy"));
    }
}

void TabStackedWidget::on_btn_Scan_clicked()
{
//    qDebug()<<"on_btn_Scan_clicked";
//    const char *image_path = "/tmp/vop_scan/2019-01-06_13-59-14-964.bmp";

//    QSize size = QSize(2496,3507);
//    ui->scrollArea_ScanImage->add_image_item(image_path ,size);
    QVariant data;
    ScanSettings paraScanSettings;
    paraScanSettings.settings = paramScan;
    data.setValue<ScanSettings>(paraScanSettings);
    gUInterface->setCurrentPrinterCmd(UIConfig::CMD_Scan,data);

    gUInterface->emitStartScanSignal();
    ui->btn_ScanSave->setEnabled(false);
    ui->scrollArea_ScanImage->setEnabled(false);
    ui->btn_MoreSetting_Scan->setEnabled(false);
    ui->btn_Scan->setEnabled(false);
    ui->btn_ScanCancel->setEnabled(true);

}

void TabStackedWidget::on_btn_MoreSetting_Scan_clicked()
{
    MoreSettingsForScan *moreSettingScan = new MoreSettingsForScan(this,&paramScan);
    connect(moreSettingScan, SIGNAL(signals_scan_buffer_size(float,int)), SLOT(slots_scan_image_size(float,int)));
    moreSettingScan->exec();
}

void TabStackedWidget::slots_scan_image_size(float size, int unit)//Added by gavin for setting scan buffer size. 2016-04-08
{
    QString labelTitle = tr("ResStr_Scanned_image_size");
    char sizeText[16] = {0};
    if(unit == 1)
    {
        sprintf(sizeText, "%0.2f%s", size, "KB");
    }
    else if(unit == 2)
    {
        sprintf(sizeText, "%0.2f%s", size, "MB");
    }
    else
    {
        sprintf(sizeText, "%d%s", (int)size, "B");
    }
    QString labelText = QString("%1%2").arg(labelTitle).arg(sizeText);

    ui->label_ImageSize->setText(labelText);

}

void TabStackedWidget::on_btn_MoreSetting_Copy_clicked()
{
    MoreSettingsForCopy *moreSettingsForCopy = new MoreSettingsForCopy(this,ui->cBox_DuplexCopy->isChecked(),ui->cBox_IsIDCard->isChecked(),isDuplexCopyDevice,&paramCopy);

    if(isDuplexCopyDevice)
    {
        moreSettingsForCopy->setMinimumHeight(649);
        moreSettingsForCopy->setMaximumHeight(649);
    }
    else
    {
        moreSettingsForCopy->setMinimumHeight(498);
        moreSettingsForCopy->setMaximumHeight(498);
    }


    moreSettingsForCopy->exec();
}

void TabStackedWidget::on_btn_DensityAdd_clicked()
{
    if (value_Density < 5)
    {
        value_Density++;
        switch (value_Density) {
        case 5:
            ui->mark4_Density->setStyleSheet("background-color: rgb(53, 177, 20);");
            break;
        case 4:
            ui->mark3_Density->setStyleSheet("background-color: rgb(53, 177, 20);");
            break;
        case 3:
            ui->mark2_Density->setStyleSheet("background-color: rgb(53, 177, 20);");
            break;
        case 2:
            ui->mark1_Density->setStyleSheet("background-color: rgb(53, 177, 20);");
            break;
        default:
            break;
        }
    }
}

void TabStackedWidget::on_btn_DensityReduce_clicked()
{
    if (value_Density > 1)
    {
        value_Density--;
        switch (value_Density) {
        case 4:
            ui->mark4_Density->setStyleSheet("background-color: rgb(154, 238, 117);");
            break;
        case 3:
            ui->mark3_Density->setStyleSheet("background-color: rgb(154, 238, 117);");
            break;
        case 2:
            ui->mark2_Density->setStyleSheet("background-color: rgb(154, 238, 117);");
            break;
        case 1:
            ui->mark1_Density->setStyleSheet("background-color: rgb(154, 238, 117);");
            break;
        default:
            break;
        }
    }
}

void TabStackedWidget::on_btn_CopyNumAdd_clicked()
{
    int value = ui->copyNum->text().toInt();
    if (value < 99)
    {
        value ++;
        QString str;
        str.setNum(value);
        ui->copyNum->setText(str);
    }
}

void TabStackedWidget::on_btn_CopyNumReduce_clicked()
{
    int value = ui->copyNum->text().toInt();
    if (value > 1)
    {
        value --;
        QString str;
        str.setNum(value);
        ui->copyNum->setText(str);
    }
}

void TabStackedWidget::setCopyStackedWidgetCurrentIndex(int index)
{
    ui->copyPage->setHidden(false);
    ui->copyStackedWidget->setCurrentIndex(index);
}

void TabStackedWidget::set_copy_enabled(bool enabled)
{
    qDebug()<<"btn_Copy";
    ui->btn_Copy->setEnabled(enabled);
}

void TabStackedWidget::set_scan_enabled(bool enabled)
{
     ui->btn_Scan->setEnabled(enabled);
}

void TabStackedWidget::set_setting_enabled(bool enabled)
{
    ui->settingStackedWidget->setEnabled(enabled);
}

void TabStackedWidget::startCycleEmit()
{
    emit cycleStartFromTab();
}

void TabStackedWidget::stopCycleEmit()
{
    emit cycleStopFromTab();
}

void TabStackedWidget::setEnabledDuplexCopy(bool enabled)
{
    qDebug()<<"setEnabledDuplexCopy"<<enabled;
    ui->text_DuplexCopy->setHidden(!enabled);
    ui->icon_DuplexCopy->setHidden(!enabled);
    ui->cBox_DuplexCopy->setHidden(!enabled);
    isDuplexCopyDevice = enabled;
}

void TabStackedWidget::setEnabledCopyScan(bool enabled)
{
    ui->copyPage->setHidden(!enabled);
    if(enabled)
    {
        ui->settingPage->setStyleSheet("#settingPage{background-image: url(:/Images/PageScan.tif);}");
    }
    else
    {
        ui->scanPage->setHidden(true);
        ui->settingPage->setStyleSheet("#settingPage{background-image: url(:/Images/PagePrint.tif);}");
    }
}

void TabStackedWidget::setEnabledWifi(bool enabled)
{
    ui->settingStackedWidget->setWifiEnabled(enabled);
    ui->btn_WiFi->setHidden(!enabled);
    ui->btn_SoftAP->setHidden(!enabled);
    ui->btn_TCPIPV4->setHidden(!enabled);
    ui->btn_TCPIPV6->setHidden(!enabled);
    ui->img_WiFi->setHidden(!enabled);
    ui->img_SoftAP->setHidden(!enabled);
    ui->img_TCPIPV4->setHidden(!enabled);
    ui->img_TCPIPV6->setHidden(!enabled);

    if(enabled)
    {
        ui->settingStackedWidget->setCurrentIndex(0);
        ui->btn_PowerSave->setGeometry(ui->btn_TCPIPV6->geometry().x(),ui->btn_TCPIPV6->geometry().y()+40,ui->btn_TCPIPV6->geometry().width(),ui->btn_TCPIPV6->geometry().height());
        ui->btn_UserConfig->setGeometry(ui->btn_TCPIPV6->geometry().x(),ui->btn_TCPIPV6->geometry().y()+80,ui->btn_TCPIPV6->geometry().width(),ui->btn_TCPIPV6->geometry().height());
        ui->btn_Password->setGeometry(ui->btn_TCPIPV6->geometry().x(),ui->btn_TCPIPV6->geometry().y()+120,ui->btn_TCPIPV6->geometry().width(),ui->btn_TCPIPV6->geometry().height());
        QRect rect = ui->img_PowerSave->geometry();
        ui->img_PowerSave->setGeometry(rect.x(),ui->btn_PowerSave->geometry().y()+(ui->btn_PowerSave->geometry().height()-rect.height())/2,rect.width(),rect.height());
        rect = ui->img_UserConfig->geometry();
        ui->img_UserConfig->setGeometry(rect.x(),ui->btn_UserConfig->geometry().y()+(ui->btn_UserConfig->geometry().height()-rect.height())/2,rect.width(),rect.height());
        rect = ui->img_Password->geometry();
        ui->img_Password->setGeometry(rect.x(),ui->btn_Password->geometry().y()+(ui->btn_Password->geometry().height()-rect.height())/2,rect.width(),rect.height());

    }
    else
    {
        ui->settingStackedWidget->setCurrentIndex(4);
        ui->btn_PowerSave->setGeometry(ui->btn_WiFi->geometry());
        ui->btn_UserConfig->setGeometry(ui->btn_SoftAP->geometry());
        ui->btn_Password->setGeometry(ui->btn_TCPIPV4->geometry());
        QRect rect = ui->img_PowerSave->geometry();
        ui->img_PowerSave->setGeometry(rect.x(),ui->btn_PowerSave->geometry().y()+(ui->btn_PowerSave->geometry().height()-rect.height())/2,rect.width(),rect.height());
        rect = ui->img_UserConfig->geometry();
        ui->img_UserConfig->setGeometry(rect.x(),ui->btn_UserConfig->geometry().y()+(ui->btn_UserConfig->geometry().height()-rect.height())/2,rect.width(),rect.height());
        rect = ui->img_Password->geometry();
        ui->img_Password->setGeometry(rect.x(),ui->btn_Password->geometry().y()+(ui->btn_Password->geometry().height()-rect.height())/2,rect.width(),rect.height());
    }
}

void TabStackedWidget::on_btn_Copy_clicked()
{
    qDebug()<<"start copy";
    if (ui->cBox_IsIDCard->isChecked() == true)
    {
        if (paramCopy.promptInfo.isIDCard == true)
        {
            bool enNextShow = true;
//            QString videoTypeStr = "01_JAM";
//            QString languageStr = "SimplifiedChinese";
            AnimationDlg *aDialog = new AnimationDlg(this, 1, &enNextShow);
            aDialog->setAttribute(Qt::WA_DeleteOnClose);
            if (aDialog->exec() == QDialog::Rejected)
            {
                return;
            }
            if(enNextShow == false)
            {
                paramCopy.promptInfo.isIDCard = false;
            }
        }

        paramCopy.scaling = 100;
        paramCopy.docDpi = DocDpi_Copy_DPI600;
        paramCopy.outputSize = OutPutSize_Copy_A4;
        paramCopy.isMultiPage = false;
        paramCopy.multiMode = TwoInOne;

    }
    else
    {
        if (paramCopy.promptInfo.isMultible == true)
        {
            bool enNextShow = true;
//            QString videoTypeStr = "06_Nin1Copy";
//            QString languageStr = "SimplifiedChinese";
            AnimationDlg *aDialog = new AnimationDlg(this, 2, &enNextShow);
            aDialog->setAttribute(Qt::WA_DeleteOnClose);
            if (aDialog->exec() == QDialog::Rejected)
            {
                return;
            }
            qDebug()<<"enNextShow"<<enNextShow;
            if(enNextShow == false)
            {
                paramCopy.promptInfo.isMultible = false;
            }
        }
    }

    copycmdset copyPara;
    copyPara.copyNum = ui->copyNum->text().toUInt();
    copyPara.Density = value_Density;
    copyPara.scanMode = paramCopy.docType;
    copyPara.orgSize = paramCopy.docSize;
    copyPara.paperSize = paramCopy.outputSize;

    if (paramCopy.isMultiPage == false && ui->cBox_IsIDCard->isChecked() == false)
    {
        copyPara.nUp = OneInOne;
    }
    else
    {
        copyPara.nUp = paramCopy.multiMode;
    }

    copyPara.dpi = paramCopy.docDpi;
    copyPara.mediaType = paramCopy.docType;
    copyPara.scale = paramCopy.scaling;

    if(isDuplexCopyDevice&&ui->cBox_IsIDCard->isChecked() == true)
    {
        copyPara.IDCardMode = paramCopy.idCardCopyMode;
    }

    QVariant data;
    data.setValue<copycmdset>(copyPara);
    gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_COPY,data);
}

#define _QT_PDF 1
#if _QT_PDF
#include <QPainter>
#include <QPrinter>

//extern void saveMultiPageTiffImage(NSString* tmpPath ,NSString* imageFilePath);
//void saveMultiPageTiffImage(const QString& tmpPath ,const QString& imageFilePath)
//{
//    return saveMultiPageTiffImage(tmpPath.toNSString() ,imageFilePath.toNSString());
//}

static QPrinter* printer = NULL;
static QPainter* painter = NULL;
void saveMultiPagePdfImageInit(const QString& fileName)
{
    printer = new QPrinter;
    printer->setPageSize(QPrinter::A4); //设置纸张大小为A4
    printer->setOutputFormat(QPrinter::PdfFormat); //设置输出格式为pdf
    printer->setOutputFileName(fileName);  //设置输出路径
    painter = new QPainter(printer);
}

bool saveMultiPagePdfImage(const QString& tmpPath ,bool firstPage)
{
    QRect rect = painter->viewport();
    if(rect.isEmpty())
        return false;
    QPixmap pixmap = QPixmap(tmpPath).scaled(rect.size() ,Qt::KeepAspectRatio);
    QSize size = pixmap.size();
    if(!size.isValid())
        return false;
    if(!firstPage)
        printer->newPage();
    painter->drawPixmap(rect.x() + (rect.width() - size.width()) / 2,
                       rect.y() + (rect.height() - size.height()) / 2,pixmap);
    return true;
}

void saveMultiPagePdfImageRelease()
{
    if(painter)
        delete painter;
    painter = NULL;
    if(printer)
        delete printer;
    printer = NULL;
}
#endif
void TabStackedWidget::on_btn_ScanSave_clicked()
{
    QList<QListWidgetItem*> item_list = ui->scrollArea_ScanImage->selectedItems();
    QString filter = tr("TIF(*.tif);;PDF(*pdf);;JPG(*jpg)");
    QString selectedFilter;
    if(item_list.count() > 1){
        filter = tr("TIF(*.tif);;PDF(*pdf)");
    }
    QString picPath = QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
    QString filename = QFileDialog::getSaveFileName(0 ,tr("Save File"), picPath,filter,&selectedFilter);
    qDebug()<<filename<<selectedFilter;
    QString temp_filename;
    if(!filename.isEmpty()){
//        if(filename.endsWith(".pdf")){
        if(selectedFilter == tr("PDF(*pdf)")){
            if(filename.endsWith(".pdf") == false)
            {
                filename = filename.append(".pdf");
            }

            qDebug()<<filename;
            saveMultiPagePdfImageInit(filename);
            int j = 0;
            foreach (QListWidgetItem* item, item_list) {
                temp_filename = item->data(Qt::UserRole).toString();
                saveMultiPagePdfImage(temp_filename ,j == 0);
                j++;
            }
            saveMultiPagePdfImageRelease();
        }else if(selectedFilter == tr("TIF(*.tif)")){
            if(filename.endsWith(".tiff"))
            {
                filename = filename.append(".tiff");
            }
            qDebug()<<filename;
            QString temp_file("tmp.tiff");
            bool first_time = true;
            QString cmd;
            foreach (QListWidgetItem* item, item_list) {
                temp_filename = item->data(Qt::UserRole).toString();
                QImage image(temp_filename);
                image.save(temp_file);
                if (first_time) {
                    cmd = QString("tiffutil -cat") + " \"" + temp_file + "\" " +"-out" + " \"" + filename + "\" ";
                    first_time = false;
                }else{
                    cmd = QString("tiffutil -cat") + " \"" + filename + "\" " + " \"" + temp_file + "\" " +"-out" + " \"" + filename + "\" ";
                }
                system(cmd.toLatin1());
                QFile(temp_file).remove();
            }
        }else{
            if(filename.endsWith(".jpg") == false)
            {
                filename = filename.append(".jpg");
            }
            qDebug()<<filename;
            foreach (QListWidgetItem* item, item_list) {
                temp_filename = item->data(Qt::UserRole).toString();
                QImage image(temp_filename);
                image.save(filename);
                break;
            }
        }
    }
}

void TabStackedWidget::on_copyNum_textChanged(const QString &arg1)
{
    qDebug()<<arg1;
    if(arg1.toInt() < 1)
    {
        ui->copyNum->setText("1");
        SettingWarming *msgWarm  = new SettingWarming(this, tr("ResStr_The_valid_range_is_1_99__please_confirm_and_enter_again_"));
        msgWarm->setWindowTitle(tr("ResStr_Error"));
        msgWarm->setWindowFlags(msgWarm->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                & ~Qt::WindowMinimizeButtonHint);
        msgWarm->exec();
    }
    else if(arg1.toInt() > 99)
    {
        ui->copyNum->setText("99");
        SettingWarming *msgWarm  = new SettingWarming(this, tr("ResStr_The_valid_range_is_1_99__please_confirm_and_enter_again_"));
        msgWarm->setWindowTitle(tr("ResStr_Error"));
        msgWarm->setWindowFlags(msgWarm->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                & ~Qt::WindowMinimizeButtonHint);
        msgWarm->exec();
    }
}

void TabStackedWidget::on_btn_ScanCancel_clicked()
{
    gUInterface->cancel_work();
}

void TabStackedWidget::on_TUSBBtn_3_clicked()
{
    QString lan = QLocale::system().name();
    QString helpPath;
    if(lan == "en_US")
    {
        helpPath = helpPath.append("/usr/share/lnthrvop/mht/HelpFile/Help/English/USB.mht");
    }else if (lan == "zh_CN")
    {
        helpPath = helpPath.append("/usr/share/lnthrvop/mht/HelpFile/Help/SimplifiedChinese/USB.mht");
    }
    qDebug()<<helpPath;
    QDesktopServices::openUrl(QUrl(helpPath));
}

void TabStackedWidget::on_TWiFiBtn_3_clicked()
{
    QString lan = QLocale::system().name();
    QString helpPath;
    if(lan == "en_US")
    {
        helpPath = "/usr/share/lnthrvop/HelpFile/mht/Help/English/Wi-Fi.mht";
    }else if (lan == "zh_CN")
    {
        helpPath = "/usr/share/lnthrvop/HelpFile/mht/Help/SimplifiedChinese/Wi-Fi.mht";
    }
    QDesktopServices::openUrl(QUrl(helpPath));
}