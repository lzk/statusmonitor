#include "tabstackedwidget.h"
#include "ui_tabstackedwidget.h"
#include "qdebug.h"

TabStackedWidget::TabStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::TabStackedWidget)
{
    ui->setupUi(this);

    //init copyNum,defaule value is 1;
    QRegExp rx("^([0-9]|[1-9]\\d)$");
    QRegExpValidator *pReg = new QRegExpValidator(rx,this);
    ui->copyNum->setValidator(pReg);
    ui->copyNum->setText(tr("1"));

    //init Density,default is level2;
    value_Density = 2;
    ui->mark0_Density->setStyleSheet("background-color: rgb(53, 177, 20);");
    ui->mark1_Density->setStyleSheet("background-color: rgb(53, 177, 20);");
    ui->mark2_Density->setStyleSheet("background-color: rgb(154, 238, 117);");
    ui->mark3_Density->setStyleSheet("background-color: rgb(154, 238, 117);");
    ui->mark4_Density->setStyleSheet("background-color: rgb(154, 238, 117);");

    selectState = "QPushButton{border-image: url(:/Images/Btn_Green.png);color:white;}";
    selectHover = "QPushButton::hover{color:black;}";
    selectPressed = "QPushButton::pressed{border-image: url(:/Images/Btn_Green_Pressed.png);color:white;}";

    unSelectState = "QPushButton{border-image: url(:/Images/Btn_Gray.png);}";
    unSelectHover = "QPushButton::hover{color: rgb(52, 212, 34);}";
    unSelectPressed = "QPushButton::pressed{border-image: url(:/Images/Btn_Gray_Pressed.png);color:white;}";

    /*settingsStackedWidget = new SettingsStackedWidget(this->widget(3));
    settingsStackedWidget->setGeometry(476, 290, 251, 301);
    ui->settingStackedWidget->hide();*/

    on_scrollArea_ScanImage_itemSelectionChanged();
    connect(ui->scrollArea_ScanImage ,SIGNAL(print_scan_images(QStringList)) ,this ,SLOT(print_scan_images(QStringList)));

    paramScan.scan_doctype = T_Photo;
    paramScan.scan_dpi = Scan_300DPI;
    paramScan.colorModel = Color;
    paramScan.contrast = 50;
    paramScan.brightness = 50;
    paramScan.scan_size = Scan_A4;

    QString labelTitle = "待扫描图片大小:";
    QString labelText = QString("%1%2").arg(labelTitle).arg("24.89MB");
    ui->label_ImageSize->setText(labelText);

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


/*void TabStackedWidget::setDefault_Copy()
{
    copycmdset para;
    copycmdset *p = &para;
    pDeviceManager->copy_get_defaultPara(p);

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

}*/

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
    ui->img_TCPIP->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password.png)");

    ui->btn_WiFi->setStyleSheet(selectState+selectHover+selectPressed);
    ui->btn_SoftAP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_PowerSave->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_UserConfig->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_Password->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);

    //neil add for settings page 2016-01-29
   // settingsStackedWidget->changeStackIndex(0);
    ui->settingStackedWidget->setCurrentIndex(0);
}

void TabStackedWidget::on_btn_SoftAP_clicked()
{
    ui->img_WiFi->setStyleSheet("border-image: url(:/Images/Wireless.png)");
    ui->img_SoftAP->setStyleSheet("border-image: url(:/Images/SoftAP_Active.png)");
    ui->img_TCPIP->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password.png)");

    ui->btn_WiFi->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_SoftAP->setStyleSheet(selectState+selectHover+selectPressed);
    ui->btn_TCPIP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_PowerSave->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_UserConfig->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_Password->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);

    //neil add for settings page 2016-01-29
   //settingsStackedWidget->changeStackIndex(1);
    ui->settingStackedWidget->setCurrentIndex(1);
}

void TabStackedWidget::on_btn_TCPIP_clicked()
{
    ui->img_WiFi->setStyleSheet("border-image: url(:/Images/Wireless.png)");
    ui->img_SoftAP->setStyleSheet("border-image: url(:/Images/SoftAP.png)");
    ui->img_TCPIP->setStyleSheet("border-image: url(:/Images/TCPIP_Active.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password.png)");

    ui->btn_WiFi->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_SoftAP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIP->setStyleSheet(selectState+selectHover+selectPressed);
    ui->btn_PowerSave->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_UserConfig->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_Password->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);

    //neil add for settings page 2016-01-29
    //settingsStackedWidget->changeStackIndex(2);
    ui->settingStackedWidget->setCurrentIndex(2);
}

void TabStackedWidget::on_btn_PowerSave_clicked()
{
    ui->img_WiFi->setStyleSheet("border-image: url(:/Images/Wireless.png)");
    ui->img_SoftAP->setStyleSheet("border-image: url(:/Images/SoftAP.png)");
    ui->img_TCPIP->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave_Active.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password.png)");

    ui->btn_WiFi->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_SoftAP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_PowerSave->setStyleSheet(selectState+selectHover+selectPressed);
    ui->btn_UserConfig->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_Password->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);

    //neil add for settings page 2016-01-29
    //settingsStackedWidget->changeStackIndex(3);
    ui->settingStackedWidget->setCurrentIndex(3);
}

void TabStackedWidget::on_btn_UserConfig_clicked()
{
    ui->img_WiFi->setStyleSheet("border-image: url(:/Images/Wireless.png)");
    ui->img_SoftAP->setStyleSheet("border-image: url(:/Images/SoftAP.png)");
    ui->img_TCPIP->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig_Active.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password.png)");

    ui->btn_WiFi->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_SoftAP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_PowerSave->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_UserConfig->setStyleSheet(selectState+selectHover+selectPressed);
    ui->btn_Password->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);

    //neil add for settings page 2016-01-29
    //settingsStackedWidget->changeStackIndex(4);
    ui->settingStackedWidget->setCurrentIndex(4);
}

void TabStackedWidget::on_btn_Password_clicked()
{
    ui->img_WiFi->setStyleSheet("border-image: url(:/Images/Wireless.png)");
    ui->img_SoftAP->setStyleSheet("border-image: url(:/Images/SoftAP.png)");
    ui->img_TCPIP->setStyleSheet("border-image: url(:/Images/TCPIP.png)");
    ui->img_PowerSave->setStyleSheet("border-image: url(:/Images/PowerSave.png)");
    ui->img_UserConfig->setStyleSheet("border-image: url(:/Images/UserConfig.png)");
    ui->img_Password->setStyleSheet("border-image: url(:/Images/Password_Active.png)");

    ui->btn_WiFi->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_SoftAP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_TCPIP->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_PowerSave->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_UserConfig->setStyleSheet(unSelectState+unSelectHover+unSelectPressed);
    ui->btn_Password->setStyleSheet(selectState+selectHover+selectPressed);

    //neil add for settings page 2016-01-29
   // settingsStackedWidget->changeStackIndex(5);
    ui->settingStackedWidget->setCurrentIndex(5);
}


void TabStackedWidget::on_cBox_IsIDCard_clicked(bool checked)
{
    if (true==checked)
    {
        ui->cBox_IsIDCard->setStyleSheet("border-image: url(:/Images/CheckBox_Open.png);");
        ui->icon_IDCardCopy->setStyleSheet("border-image: url(:/Images/IDCardCopyIconEnable.png);");
        ui->btn_Copy->setText("身份证复印");

        ui->cBox_DuplexCopy->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
        ui->cBox_DuplexCopy->setChecked(false);
        ui->icon_DuplexCopy->setStyleSheet("border-image: url(:/Images/DulplexCopyIconDisable.tif);");
    }
    else
    {
        ui->cBox_IsIDCard->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
        ui->icon_IDCardCopy->setStyleSheet("border-image: url(:/Images/IDCardCopyIconDisable.png);");
        ui->btn_Copy->setText("复印");
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
        ui->btn_Copy->setText("双面复印");
    }
    else
    {
        ui->cBox_DuplexCopy->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
        ui->icon_DuplexCopy->setStyleSheet("border-image: url(:/Images/DulplexCopyIconDisable.tif);");
        ui->btn_Copy->setText("复印");
    }
}



void TabStackedWidget::on_btn_Scan_clicked()
{
    const char *image_path = "/tmp/vop_scan/123456.bmp";

/*    QImage *image = new QImage;

    if(image->load(image_path)== false)
    {
        qDebug()<<"load image error.";
    }
    else
    {
        ui->label_Background_2->setPixmap(QPixmap::fromImage(*image));
    }*/

    QSize size = QSize(850,640);
    ui->scrollArea_ScanImage->add_image_item(image_path ,size);
}

void TabStackedWidget::on_btn_MoreSetting_Scan_clicked()
{
    MoreSettingsForScan *moreSettingScan = new MoreSettingsForScan(this,&paramScan);
    connect(moreSettingScan, SIGNAL(signals_scan_buffer_size(float,int)), SLOT(slots_scan_image_size(float,int)));
    moreSettingScan->exec();
    moreSettingScan->deleteLater();
}

void TabStackedWidget::on_btn_MoreSetting_Copy_clicked()
{
    MoreSettingsForCopy *moreSettingsForCopy = new MoreSettingsForCopy(0,ui->cBox_IsIDCard->isChecked(),&paramCopy);
    moreSettingsForCopy->exec();
    moreSettingsForCopy->deleteLater();
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
