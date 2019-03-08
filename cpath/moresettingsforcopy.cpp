#include "moresettingsforcopy.h"
#include "ui_moresettingsforcopy.h"
#include <QStandardItemModel>
#include <QListView>
#include <QPalette>

#define MIN_SCALING 25
#define MAX_SCALING 400

MoreSettingsForCopy::MoreSettingsForCopy(QWidget *parent,bool duplexCopyFlag, bool idCardFlag,bool isDuplexCopyDevice, Param_Copy *pParam) :
    QDialog(parent),
    ui(new Ui::MoreSettingsForCopy)
{
    timer = new QTimer(this);
    _idCardFlag = idCardFlag;
    _duplexCopyFlag = duplexCopyFlag;
    _isDuplexCopyDevice = isDuplexCopyDevice;

    ParamForCopy = pParam;

    if(NULL != pParam)
    {
        ParamForCopy = pParam;
    }else
    {
        ParamForCopy = &defParam;
    }

    ui->setupUi(this);

    ui->btClose->hide();
    ui->label_title->hide();

    ui->label_tip->hide();
    ui->scaling->setValidator(new QIntValidator(0,10000,this));
    ui->scaling->installEventFilter(this);

    if(_isDuplexCopyDevice)
    {
        ui->Label_isIDCardCopyMode->show();
        ui->label_A4_1->show();
        ui->label_A4_2->show();
        ui->label_A4_3->show();
        ui->label_A4_4->show();
        ui->label_A4_5->show();
        ui->label_A4_6->show();
        ui->label_A4_7->show();
        ui->label_A4_8->show();
        ui->btA4_1->show();
        ui->btA4_2->show();
        ui->btA4_3->show();
        ui->btA4_4->show();

        this->setStyleSheet("#MoreSettingsForCopy{background-image: url(:/Images/moreSettings_Copy_1.png);background-repeat: repeat;border-image: url();}");
        QRect geometry = ui->btOK->geometry();
        geometry.setY(590);
        ui->btOK->setGeometry(geometry);
        geometry = ui->btDefault->geometry();
        geometry.setY(590);
        ui->btDefault->setGeometry(geometry);

        enableIDCardCopyMode(_idCardFlag);

    }else
    {
        ui->Label_isIDCardCopyMode->hide();
        ui->label_A4_1->hide();
        ui->label_A4_2->hide();
        ui->label_A4_3->hide();
        ui->label_A4_4->hide();
        ui->label_A4_5->hide();
        ui->label_A4_6->hide();
        ui->label_A4_7->hide();
        ui->label_A4_8->hide();
        ui->btA4_1->hide();
        ui->btA4_2->hide();
        ui->btA4_3->hide();
        ui->btA4_4->hide();

        this->setStyleSheet("#MoreSettingsForCopy{background-image: url(:/Images/moreSettings_Copy.png);background-repeat: repeat;border-image: url();}");
        QRect geometry = ui->btOK->geometry();
        ui->btOK->setGeometry(geometry.x(),431,geometry.width(),geometry.height());
        geometry = ui->btDefault->geometry();
        ui->btDefault->setGeometry(geometry.x(),431,geometry.width(),geometry.height());

    }


    if(_idCardFlag)
    {
        ParamForCopy->scaling = 100;
//        ParamForCopy->docType = DocType_Copy_Photo;
        ParamForCopy->docDpi = DocDpi_Copy_DPI300;
        ParamForCopy->outputSize = OutPutSize_Copy_A4;
        ParamForCopy->isMultiPage = false;
        ParamForCopy->multiMode = TwoInOne;
    }
    else if(_duplexCopyFlag)
    {
        ParamForCopy->isMultiPage = false;
        ParamForCopy->multiMode = TwoInOne;
        ParamForCopy->outputSize = OutPutSize_Copy_A4;
        if(ParamForCopy->docSize == DocSize_Copy_Executive)
        {
            ParamForCopy->docSize = DocSize_Copy_A4;
        }
        ParamForCopy->scaling = getScalingValue(ParamForCopy->outputSize,ParamForCopy->docSize);

        if(ParamForCopy->paperType > MediaType_Copy_Recycled)
        {
            ParamForCopy->paperType = MediaType_Copy_Plain;
        }
    }
    showParam(ParamForCopy);

    QListView *listView = new QListView(ui->outPutSizeList);
    listView->setStyleSheet("QListView::item:selected{background-color:gray;color:white}");
    ui->outPutSizeList->setView(listView);

    QListView *listView2 = new QListView(ui->docSizeList);
    listView2->setStyleSheet("QListView::item:selected{background-color:gray;color:white}");
    ui->docSizeList->setView( listView2);
    QListView *listView3 = new QListView(ui->dpiList);
    listView3->setStyleSheet("QListView::item:selected{background-color:gray;color:white}");
    ui->dpiList->setView(listView3);
    QListView *listView4 = new QListView(ui->paperTypeList);
    listView4->setStyleSheet("QListView::item:selected{background-color:gray;color:white}");
    ui->paperTypeList->setView(listView4);
}
void MoreSettingsForCopy::setDefault()
{
    copycmdset para;
    copycmdset *p = &para;
    memcpy(p ,&default_copy_parameter ,sizeof(default_copy_parameter));

    Param_Copy tmpParam;
    Param_Copy *defaultParamForCopy = &tmpParam;
//    if(!_idCardFlag)
//    {
//        defaultParamForCopy->scaling = (int)p->scale;
//        defaultParamForCopy->docType = (DocType_Copy)p->scanMode;
//        defaultParamForCopy->docSize = (DocSize_Copy)p->orgSize;
//        defaultParamForCopy->docDpi = (DocDpi_Copy)p->dpi;
//        defaultParamForCopy->outputSize = (OutPutSize_Copy)p->paperSize;
//        defaultParamForCopy->paperType = (MediaType_Copy)p->mediaType;
//        defaultParamForCopy->isMultiPage = false;
//        defaultParamForCopy->multiMode = (MultiMode_Copy)p->nUp;

//        defaultParamForCopy->promptInfo.isIDCard = true;
//        defaultParamForCopy->promptInfo.isMultible = true;

//        showParam(defaultParamForCopy);
//    }else
//    {
//        defaultParamForCopy->docType = (DocType_Copy)p->scanMode;
//        defaultParamForCopy->paperType = (MediaType_Copy)p->mediaType;

//        defaultParamForCopy->promptInfo.isIDCard = true;
//        defaultParamForCopy->promptInfo.isMultible = true;
//        showParam(defaultParamForCopy);
//    }
    defaultParamForCopy->scaling = (int)p->scale;
    defaultParamForCopy->docType = (DocType_Copy)p->scanMode;
    defaultParamForCopy->docSize = (DocSize_Copy)p->orgSize;
    defaultParamForCopy->docDpi = (DocDpi_Copy)p->dpi;
    defaultParamForCopy->outputSize = (OutPutSize_Copy)p->paperSize;
    defaultParamForCopy->paperType = (MediaType_Copy)p->mediaType;
    defaultParamForCopy->isMultiPage = false;
    defaultParamForCopy->multiMode = (MultiMode_Copy)p->nUp;
    _multiMode = (MultiMode_Copy)p->nUp;

    defaultParamForCopy->promptInfo.isIDCard = true;
    defaultParamForCopy->promptInfo.isMultible = true;

    showParam(defaultParamForCopy);

}

void MoreSettingsForCopy::showParam(Param_Copy *param)
{
    QString text;
    if(_idCardFlag)
    {
        ui->label_1->setDisabled(true);
        ui->label_3->setDisabled(true);
        //ui->label_7->setDisabled(true);
        ui->isNinOne->setDisabled(true);

        ui->btReduce->setDisabled(true);
        ui->btAdd->setDisabled(true);
        ui->label->setDisabled(true);


        ui->scaling->setText(text.setNum(param->scaling));
        ui->scaling->setDisabled(true);

        if(param->docType == DocType_Copy_Photo)
        {
            ui->btPicture->setChecked(true);
            ui->btText->setChecked(false);
        }else
        {
            ui->btPicture->setChecked(false);
            ui->btText->setChecked(true);
        }

        ui->docSizeList->setCurrentIndex(param->docSize);
        ui->docSizeList->setDisabled(true);
        ui->dpiList->setCurrentIndex(param->docDpi);
        ui->outPutSizeList->setCurrentIndex(param->outputSize);
        ui->paperTypeList->setCurrentIndex(param->paperType);
        ui->label_20->setStyleSheet("#label_20 {background-color: rgb(198, 198, 198);border-radius:8px;}");
        ui->label_21->setStyleSheet("#label_21 {background-color: rgb(198, 198, 198);border-radius:8px;}");
        ui->label_22->setStyleSheet("#label_22 {background-color: rgb(198, 198, 198);border-radius:8px;}");


        //设置outPutSizeList的出了a4其他都不可用
        QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->outPutSizeList->model());
        for(int i=0; i<8; i++)
        {
            if((i != 1) && (model != NULL))
            {
                model->item(i)->setEnabled(false);
               //此处设置combobox的下拉选项字体显示灰色
               // ui->outPutSizeList->setItemData(i,QColor(207,207,207),Qt::ForegroundRole);
            }
        }

         QStandardItemModel *model2 = qobject_cast<QStandardItemModel *>(ui->dpiList->model());
         if(model2) model2->item(1)->setEnabled(false);

        if(param->isMultiPage)
        {
             ui->isNinOne->setChecked(true);
             selectMode(param->multiMode);
        }
        else
        {
            ui->isNinOne->setChecked(false);
        }

        ui->isNinOne->setDisabled(true);

//        ParamForCopy->promptInfo.isIDCard = !ParamForCopy->promptInfo.isIDCard;
        ui->btID->setChecked(param->promptInfo.isIDCard);  //ParamForCopy->promptInfo.isIDCard = true;
        on_btID_clicked(param->promptInfo.isIDCard);
//        ParamForCopy->promptInfo.isMultible = !ParamForCopy->promptInfo.isMultible;
        ui->btNInOne->setChecked(param->promptInfo.isMultible);  //ParamForCopy->promptInfo.isMultible
        on_btNInOne_clicked(param->promptInfo.isMultible);

        if(_isDuplexCopyDevice)
        {
            selectIDCardCopyMode(param->idCardCopyMode);
        }

    }else if (_duplexCopyFlag)
    {

        if(param->docType == DocType_Copy_Photo)
        {
            ui->btPicture->setChecked(true);
            ui->btText->setChecked(false);
        }else
        {
            ui->btPicture->setChecked(false);
            ui->btText->setChecked(true);
        }
        ui->docSizeList->setCurrentIndex(param->docSize);
        ui->dpiList->setCurrentIndex(param->docDpi);
        ui->outPutSizeList->setCurrentIndex(param->outputSize);
        ui->paperTypeList->setCurrentIndex(param->paperType);

        if(param->isMultiPage)
        {
             ui->isNinOne->setChecked(true);
             selectMode(param->multiMode);
        }
        else
        {
            ui->isNinOne->setChecked(false);
        }

        ui->isNinOne->setEnabled(false);

//        //设置documentSizeList的出了letter,a4,a5,b5其他都不可用
//        QStandardItemModel *model0 = qobject_cast<QStandardItemModel *>(ui->docSizeList->model());
//        for(int i=0; i<8; i++)
//        {
//            if((i == 4) && (model0 != NULL))
//            {
//                model0->item(i)->setEnabled(false);
//               //此处设置combobox的下拉选项字体显示灰色
//               // ui->outPutSizeList->setItemData(i,QColor(207,207,207),Qt::ForegroundRole);
//            }
//        }

        //设置outPutSizeList的出了a4其他都不可用
        QStandardItemModel *model1 = qobject_cast<QStandardItemModel *>(ui->outPutSizeList->model());
        for(int i=0; i<8; i++)
        {
            if((i!=1) && (model1 != NULL))
            {
                model1->item(i)->setEnabled(false);
               //此处设置combobox的下拉选项字体显示灰色
               // ui->outPutSizeList->setItemData(i,QColor(207,207,207),Qt::ForegroundRole);
            }
        }

        //设置papetype的出了plain paper,recycled paper其他都不可用
        QStandardItemModel *model2 = qobject_cast<QStandardItemModel *>(ui->paperTypeList->model());
        for(int i=0; i<5; i++)
        {
            if((i != 0)&&(i != 1) && (model2 != NULL))
            {
                model2->item(i)->setEnabled(false);
               //此处设置combobox的下拉选项字体显示灰色
               // ui->outPutSizeList->setItemData(i,QColor(207,207,207),Qt::ForegroundRole);
            }
        }

//        ParamForCopy->promptInfo.isIDCard = !ParamForCopy->promptInfo.isIDCard;
        ui->btID->setChecked(param->promptInfo.isIDCard);   //ParamForCopy->promptInfo.isIDCard = true;
        on_btID_clicked(param->promptInfo.isIDCard);
//        ParamForCopy->promptInfo.isMultible = !ParamForCopy->promptInfo.isMultible;
        ui->btNInOne->setChecked(param->promptInfo.isMultible);  //ParamForCopy->promptInfo.isMultible
        on_btNInOne_clicked(param->promptInfo.isMultible);

        ui->scaling->setText(text.setNum(param->scaling));
    }
    else
    {
        if(param->docType == DocType_Copy_Photo)
        {
            ui->btPicture->setChecked(true);
            ui->btText->setChecked(false);
        }else
        {
            ui->btPicture->setChecked(false);
            ui->btText->setChecked(true);
        }
        ui->docSizeList->setCurrentIndex(param->docSize);
        ui->dpiList->setCurrentIndex(param->docDpi);
        ui->outPutSizeList->setCurrentIndex(param->outputSize);
        ui->paperTypeList->setCurrentIndex(param->paperType);

        if(param->isMultiPage)
        {
             ui->isNinOne->setChecked(true);
             selectMode(param->multiMode);

             //设置outPutSizeList的a6，b6两种格式不可用
             QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->outPutSizeList->model());
             if(model)
             {
                 model->item(3)->setEnabled(false);
                 model->item(5)->setEnabled(false);
             }
        }
        else
        {
            ui->isNinOne->setChecked(false);
            ui->label_20->setStyleSheet("#label_20 {background-color: rgb(198, 198, 198);border-radius:8px;}");
            ui->label_21->setStyleSheet("#label_21 {background-color: rgb(198, 198, 198);border-radius:8px;}");
            ui->label_22->setStyleSheet("#label_22 {background-color: rgb(198, 198, 198);border-radius:8px;}");
        }

//        ParamForCopy->promptInfo.isIDCard = !ParamForCopy->promptInfo.isIDCard;
        ui->btID->setChecked(param->promptInfo.isIDCard);  //ParamForCopy->promptInfo.isIDCard = true;
        on_btID_clicked(param->promptInfo.isIDCard);
//        ParamForCopy->promptInfo.isMultible = !ParamForCopy->promptInfo.isMultible;
        ui->btNInOne->setChecked(param->promptInfo.isMultible);  //ParamForCopy->promptInfo.isMultible
        on_btNInOne_clicked(param->promptInfo.isMultible);

        ui->scaling->setText(text.setNum(param->scaling));
    }
}

MoreSettingsForCopy::~MoreSettingsForCopy()
{
    delete ui;
}

void MoreSettingsForCopy::on_btOK_clicked()
{
//    QDebug() << "here is on_btOK_clicked()";
    //printf("hello");

    ParamForCopy->scaling = ui->scaling->text().toInt();
    if(ui->btPicture->isChecked())
    {
        ParamForCopy->docType = DocType_Copy_Photo;
    }else if(ui->btText->isChecked())
    {
        ParamForCopy->docType = DocType_Copy_Text;
    }
    ParamForCopy->docSize = DocSize_Copy(ui->docSizeList->currentIndex());
    ParamForCopy->docDpi = DocDpi_Copy(ui->dpiList->currentIndex());
    ParamForCopy->outputSize = OutPutSize_Copy(ui->outPutSizeList->currentIndex());
    ParamForCopy->paperType = MediaType_Copy(ui->paperTypeList->currentIndex());
    ParamForCopy->isMultiPage = ui->isNinOne->isChecked();
    if(ParamForCopy->isMultiPage)
    {
        ParamForCopy->multiMode = _multiMode;
    }
    ParamForCopy->promptInfo.isIDCard = ui->btID->isChecked();
    qDebug()<<"ParamForCopy->promptInfo.isIDCard"<<ParamForCopy->promptInfo.isIDCard;
    ParamForCopy->promptInfo.isMultible = ui->btNInOne->isChecked();

    if(_idCardFlag && _isDuplexCopyDevice)
    {
        ParamForCopy->idCardCopyMode = _idCardCopyMode;
    }

    this->close();
}

void MoreSettingsForCopy::on_btDefault_clicked()
{
    setDefault();
}


//针对btReduce的点击及长按事件响应
void MoreSettingsForCopy::on_btReduce_clicked()
{
    QString text =  ui->scaling->text();
    int tmp = text.toInt();

    if((tmp > MIN_SCALING)&&(tmp <= MAX_SCALING)&&(!ParamForCopy->isMultiPage))
    {
        ui->label_tip->hide();
        tmp--;
        ui->scaling->setText(text.setNum(tmp));
    }else
    {
        if(!ParamForCopy->isMultiPage)
        {
        //    ParamForCopy->scaling = 100 - 1;
        //    ui->scaling->setText(text.setNum(ParamForCopy->scaling));
            ui->label_tip->hide();
        }
    }
}

void MoreSettingsForCopy::on_timeout_reduce()
{
    timeCount += 1;
    if(timeCount > 9)
    {
        on_btReduce_clicked();
    }
}
void MoreSettingsForCopy::on_btReduce_pressed()
{

    connect(timer, SIGNAL(timeout()), this, SLOT(on_timeout_reduce()));
    timeCount = 0;
    timer->start(80);
}

void MoreSettingsForCopy::on_btReduce_released()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(on_timeout_reduce()));
    timer->stop();
}

//针对btReduce的点击及长按事件响应
void MoreSettingsForCopy::on_btAdd_clicked()
{
    QString text =  ui->scaling->text();
    int tmp = text.toInt();

    if((tmp >= MIN_SCALING)&&(tmp < MAX_SCALING) && (!ui->isNinOne->isChecked()))
    {
        ui->label_tip->hide();
        tmp++;
        ui->scaling->setText(text.setNum(tmp));
    }else
    {
        if(!ui->isNinOne->isChecked())
        {
        //    ParamForCopy->scaling = 100 + 1;
        //    ui->scaling->setText(text.setNum(ParamForCopy->scaling));
            ui->label_tip->hide();
        }
    }

}

void MoreSettingsForCopy::on_timeout_add()
{
    timeCount += 1;
    if(timeCount > 9)
    {
        on_btAdd_clicked();
    }
}
void MoreSettingsForCopy::on_btAdd_pressed()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timeout_add()));
    timeCount = 0;
    timer->start(80);
}

void MoreSettingsForCopy::on_btAdd_released()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(on_timeout_add()));
    timer->stop();
}

/***************************************
*    name:       on_scaling_editingFinished
*    param:      void
*    function:   check for the input num and make sure the value was in the range;
****************************************/
void MoreSettingsForCopy::on_scaling_editingFinished()
{
    QString text = ui->scaling->text();
    int tmp = text.toInt();

    if(tmp > MAX_SCALING || tmp < MIN_SCALING)
    {
        ui->scaling->setText(text.setNum(100));
//        ParamForCopy->scaling = 100;
        ui->scaling->setStyleSheet("#scaling {border:transparent;}");
        ui->btReduce->setEnabled(true);
        ui->btAdd->setEnabled(true);
        ui->btOK->setEnabled(true);
    }
    else{
//        ParamForCopy->scaling = tmp;
        ui->scaling->setStyleSheet("#scaling {border:transparent;}");
     }
}



void MoreSettingsForCopy::on_scaling_textEdited(const QString &arg1)
{
    int tmp = arg1.toInt();

    if(tmp < MIN_SCALING || tmp > MAX_SCALING)
    {
        ui->label_tip->show();
        ui->scaling->setStyleSheet("#scaling {border:2px solid red;}");
        ui->btReduce->setEnabled(false);
        ui->btAdd->setEnabled(false);
        ui->btOK->setEnabled(false);
    }
    else
    {
        ui->label_tip->hide();
        ui->scaling->setStyleSheet("#scaling {border:transparent;}");
        ui->btReduce->setEnabled(true);
        ui->btAdd->setEnabled(true);
        ui->btOK->setEnabled(true);
    }
}


/***************************************
*    name:       on_isIDCardCopyMode_toggled
*    param:      bool checked;
*   function:   enable the N in One function and show the early selection;
*
****************************************/
void MoreSettingsForCopy::enableIDCardCopyMode(bool checked)
{
    QStandardItemModel *model1 = qobject_cast<QStandardItemModel *>(ui->docSizeList->model());
    QStandardItemModel *model2 = qobject_cast<QStandardItemModel *>(ui->outPutSizeList->model());
    if(checked)
    {
        ui->Label_isIDCardCopyMode->setEnabled(true);
        ui->label_A4_5->setEnabled(true);
        ui->label_A4_6->setEnabled(true);
        ui->label_A4_7->setEnabled(true);
        ui->label_A4_8->setEnabled(true);

//        ParamForCopy->isIDCardCopyMode = true;

       switch (_idCardCopyMode)
       {
           case A4Mode1: ui->label_A4_1->setStyleSheet("#label_A4_1 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
           case A4Mode2: ui->label_A4_2->setStyleSheet("#label_A4_2 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
           case A4Mode3: ui->label_A4_3->setStyleSheet("#label_A4_3 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
           case A5Mode: ui->label_A4_4->setStyleSheet("#label_A4_4 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
           default:break;
       }

       model1->item(1)->setEnabled(false);
       model1->item(2)->setEnabled(false);
       model1->item(3)->setEnabled(false);
       model1->item(4)->setEnabled(false);

       model2->item(0)->setEnabled(false);
       model2->item(2)->setEnabled(false);
       model2->item(3)->setEnabled(false);
       model2->item(4)->setEnabled(false);
       model2->item(5)->setEnabled(false);
       model2->item(6)->setEnabled(false);
       model2->item(7)->setEnabled(false);

       ui->docSizeList->setCurrentIndex(0);
       ui->outPutSizeList->setCurrentIndex(1);
    }
    else{
        ui->Label_isIDCardCopyMode->setEnabled(false);
        ui->label_A4_5->setEnabled(false);
        ui->label_A4_6->setEnabled(false);
        ui->label_A4_7->setEnabled(false);
        ui->label_A4_8->setEnabled(false);

//        ParamForCopy->isIDCardCopyMode = false;

        switch (_idCardCopyMode)
        {
            case A4Mode1: ui->label_A4_1->setStyleSheet("#label_A4_1 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
            case A4Mode2: ui->label_A4_2->setStyleSheet("#label_A4_2 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
            case A4Mode3: ui->label_A4_3->setStyleSheet("#label_A4_3 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
            case A5Mode: ui->label_A4_4->setStyleSheet("#label_A4_4 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
            default:break;
        }
        model1->item(1)->setEnabled(true);
        model1->item(2)->setEnabled(true);
        model1->item(3)->setEnabled(true);
        model1->item(4)->setEnabled(true);

        model2->item(0)->setEnabled(true);
        model2->item(2)->setEnabled(true);
        model2->item(3)->setEnabled(true);
        model2->item(4)->setEnabled(true);
        model2->item(5)->setEnabled(true);
        model2->item(6)->setEnabled(true);
        model2->item(7)->setEnabled(true);


    }
}

/***************************************
*    name:       Credentials Duplex Copy Mode
*    param1:     MultiMode checked;
*   function:    Mode support 4 options: A4 mode1, A4 mode2, A4 mode3, A5 mode.
*
****************************************/
void MoreSettingsForCopy::selectIDCardCopyMode(int mode)
{
    QStandardItemModel *model1 = qobject_cast<QStandardItemModel *>(ui->docSizeList->model());
    QStandardItemModel *model2 = qobject_cast<QStandardItemModel *>(ui->outPutSizeList->model());
    switch (_idCardCopyMode)
    {
        case A4Mode1: ui->label_A4_1->setStyleSheet("#label_A4_1 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
        case A4Mode2: ui->label_A4_2->setStyleSheet("#label_A4_2 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
        case A4Mode3: ui->label_A4_3->setStyleSheet("#label_A4_3 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
        case A5Mode: ui->label_A4_4->setStyleSheet("#label_A4_4 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
        default:break;
    }

    _idCardCopyMode = mode;

    switch (_idCardCopyMode)
    {
        case A4Mode1: ui->label_A4_1->setStyleSheet("#label_A4_1 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
        case A4Mode2: ui->label_A4_2->setStyleSheet("#label_A4_2 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
        case A4Mode3: ui->label_A4_3->setStyleSheet("#label_A4_3 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
        case A5Mode: ui->label_A4_4->setStyleSheet("#label_A4_4 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
        default:break;
    }

    if(_idCardCopyMode == A5Mode)
    {
        model1->item(0)->setEnabled(false);
        model1->item(2)->setEnabled(false);
        model1->item(3)->setEnabled(false);
        model1->item(4)->setEnabled(false);

        model2->item(0)->setEnabled(false);
        model2->item(1)->setEnabled(false);
        model2->item(3)->setEnabled(false);
        model2->item(4)->setEnabled(false);
        model2->item(5)->setEnabled(false);
        model2->item(6)->setEnabled(false);
        model2->item(7)->setEnabled(false);
//        ui->docSizeList->setCurrentIndex(0);
        ui->outPutSizeList->setCurrentIndex(2);
    }else
    {
        model1->item(1)->setEnabled(false);
        model1->item(2)->setEnabled(false);
        model1->item(3)->setEnabled(false);
        model1->item(4)->setEnabled(false);

        model2->item(0)->setEnabled(false);
        model2->item(2)->setEnabled(false);
        model2->item(3)->setEnabled(false);
        model2->item(4)->setEnabled(false);
        model2->item(5)->setEnabled(false);
        model2->item(6)->setEnabled(false);
        model2->item(7)->setEnabled(false);
//        ui->docSizeList->setCurrentIndex(0);
        ui->outPutSizeList->setCurrentIndex(1);
    }
}

void MoreSettingsForCopy::on_btA4_1_clicked()
{
    if(_idCardFlag&&_isDuplexCopyDevice)    selectIDCardCopyMode(A4Mode1);
}

void MoreSettingsForCopy::on_btA4_2_clicked()
{
    if(_idCardFlag&&_isDuplexCopyDevice)    selectIDCardCopyMode(A4Mode2);
}

void MoreSettingsForCopy::on_btA4_3_clicked()
{
    if(_idCardFlag&&_isDuplexCopyDevice)    selectIDCardCopyMode(A4Mode3);
//     ui->label_30->setText("here");
}

void MoreSettingsForCopy::on_btA4_4_clicked()
{
    if(_idCardFlag&&_isDuplexCopyDevice)    selectIDCardCopyMode(A5Mode);
}

/***************************************
*    name:       on_isNinOne_toggled
*    param:      bool checked;
*   function:   enable the N in One function and show the early selection;
*
****************************************/
void MoreSettingsForCopy::on_isNinOne_toggled(bool checked)
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->outPutSizeList->model());
    if(checked)
    {
        if(ui->outPutSizeList->currentIndex() == 0 || ui->outPutSizeList->currentIndex() == 1)
        {
            ui->label_30->setEnabled(true);
            ui->label_31->setEnabled(true);
            ui->label_32->setEnabled(true);
            ui->scaling->setDisabled(true);
            ui->label_1->setDisabled(true);
            ui->label->setDisabled(true);

            if(!_multiMode)
            {
                _multiMode = TwoInOne;
            }
           switch (_multiMode)
           {
               case TwoInOne: ui->label_20->setStyleSheet("#label_20 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
               case FourInOne: ui->label_21->setStyleSheet("#label_21 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
               case NineInOne: ui->label_22->setStyleSheet("#label_22 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
               default:break;
           }

           model->item(3)->setEnabled(false);
           model->item(5)->setEnabled(false);
        }
        else
        {
            ui->label_30->setEnabled(true);
            ui->label_1->setDisabled(true);
            ui->label->setDisabled(true);
            ui->bt4in1->setEnabled(false);
            ui->bt9in1->setEnabled(false);
            _multiMode = TwoInOne;
            ui->label_20->setStyleSheet("#label_20 {background-color: rgb(157, 157, 157);border-radius:8px;}");
            model->item(3)->setEnabled(false);
            model->item(5)->setEnabled(false);
        }
    }
    else{
        ui->label_30->setDisabled(true);
        ui->label_31->setDisabled(true);
        ui->label_32->setDisabled(true);
        ui->scaling->setEnabled(true);
        ui->label_1->setEnabled(true);
        ui->label->setEnabled(true);
        ui->bt4in1->setEnabled(true);
        ui->bt9in1->setEnabled(true);

        switch (_multiMode)
        {
            case TwoInOne: ui->label_20->setStyleSheet("#label_20 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
            case FourInOne: ui->label_21->setStyleSheet("#label_21 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
            case NineInOne: ui->label_22->setStyleSheet("#label_22 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
            default:break;
        }
        for(int i = 0; i < model->rowCount();i++)
        {
            model->item(i)->setEnabled(true);
        }
    }
}

/***************************************
*    name:       selectMode
*    param1:     MultiMode checked;
*   function:    enable the N in One function and show the early selection;
*
****************************************/
void MoreSettingsForCopy::selectMode(MultiMode_Copy mode)
{
    switch (_multiMode)
    {
        case TwoInOne: ui->label_20->setStyleSheet("#label_20 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
        case FourInOne: ui->label_21->setStyleSheet("#label_21 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
        case NineInOne: ui->label_22->setStyleSheet("#label_22 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
        default:break;
    }

    _multiMode = mode;

    switch (_multiMode)
    {
        case TwoInOne:  ui->label_20->setStyleSheet("#label_20 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
        case FourInOne: ui->label_21->setStyleSheet("#label_21 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
        case NineInOne: ui->label_22->setStyleSheet("#label_22 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
        default:break;
    }
}

void MoreSettingsForCopy::on_bt2in1_clicked()
{
    if(ui->isNinOne->isChecked())
    {
        selectMode(TwoInOne);
        QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->outPutSizeList->model());
        for(int i = 0; i < model->rowCount();i++)
        {
            if(i == 3 || i == 5)
            {
                model->item(i)->setEnabled(false);
            }
            else
            {
                model->item(i)->setEnabled(true);
            }
        }
    }
}

void MoreSettingsForCopy::on_bt4in1_clicked()
{
    if(ui->isNinOne->isChecked())
    {
        selectMode(FourInOne);
        QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->outPutSizeList->model());
        for(int i = 0; i < model->rowCount();i++)
        {
            if(i == 0 || i == 1)
            {
                model->item(i)->setEnabled(true);
            }
            else
            {
                model->item(i)->setEnabled(false);
            }
        }
    }
//     ui->label_30->setText("here");
}

void MoreSettingsForCopy::on_bt9in1_clicked()
{
    if(ui->isNinOne->isChecked())
    {
        selectMode(NineInOne);
        QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->outPutSizeList->model());
        for(int i = 0; i < model->rowCount();i++)
        {
            if(i == 0 || i == 1)
            {
                model->item(i)->setEnabled(true);
            }
            else
            {
                model->item(i)->setEnabled(false);
            }
        }
    }
}

void MoreSettingsForCopy::on_btID_clicked(bool checked)
{
    qDebug()<<"on_btID_clicked"<<checked;
//    ParamForCopy->promptInfo.isIDCard = !ParamForCopy->promptInfo.isIDCard;
//    ui->btID->setChecked(!checked);
    if(checked)
    {
        ui->btID->setStyleSheet("#btID {border-image: url(:/Images/CheckBox_Open.png);}");
    }
    else
    {
        ui->btID->setStyleSheet("#btID {border-image: url(:/Images/CheckBox_Close.png);}");
    }
}

void MoreSettingsForCopy::on_btNInOne_clicked(bool checked)
{
//    ParamForCopy->promptInfo.isMultible = !ParamForCopy->promptInfo.isMultible;
//    ui->btNInOne->setChecked(!checked);
    if(checked)
    {
        ui->btNInOne->setStyleSheet("#btNInOne {border-image: url(:/Images/CheckBox_Open.png);}");
    }
    else
    {
        ui->btNInOne->setStyleSheet("#btNInOne {border-image: url(:/Images/CheckBox_Close.png);}");
    }
}

//void MoreSettingsForCopy::on_btText_toggled(bool checked)
//{
//    if(checked)
//    {
//        ParamForCopy->docType = DocType_Copy_Text;
//    }
//}

//void MoreSettingsForCopy::on_btPicture_toggled(bool checked)
//{
//    if(checked)
//    {
//        ParamForCopy->docType = DocType_Copy_Photo;
//    }
//}

void MoreSettingsForCopy::on_docSizeList_currentIndexChanged(int index)
{
    int scaling = getScalingValue(ui->outPutSizeList->currentIndex(),index);
    QString text;
    ui->scaling->setText(text.setNum(scaling));
}

void MoreSettingsForCopy::on_dpiList_currentIndexChanged(int index)
{
    if(_idCardFlag)
    {
         ui->dpiList->setCurrentIndex(1);
    }
}

void MoreSettingsForCopy::on_outPutSizeList_currentIndexChanged(int index)
{
    if(_idCardFlag)
    {
 //       ui->outPutSizeList->setCurrentIndex(ParamForCopy->outputSize);
    }else
    {
        if(index==3 || index==5 )
        {
            ui->isNinOne->setEnabled(false);
        }
        else
        {
            ui->isNinOne->setEnabled(true);
            if(ui->isNinOne->isChecked())
            {
                if(index == 0 || index == 1)
                {
                    ui->bt4in1->setEnabled(true);
                    ui->bt9in1->setEnabled(true);
                    ui->label_31->setEnabled(true);
                    ui->label_32->setEnabled(true);
                }
                else
                {
                    ui->bt4in1->setEnabled(false);
                    ui->bt9in1->setEnabled(false);
                    ui->label_31->setEnabled(false);
                    ui->label_32->setEnabled(false);
                }
                ui->label_30->setEnabled(true);
            }
        }
    }
    int scaling = getScalingValue(index,ui->docSizeList->currentIndex());
    QString text;
    ui->scaling->setText(text.setNum(scaling));
}

int MoreSettingsForCopy::getScalingValue(int outputSize, int inputSize)
{
    qDebug()<<outputSize<<" "<<inputSize;
    int calculateScaling[8][5] =
    {
    {94, 134, 109, 100, 105},
    {100, 143, 116, 97, 112},
    {69, 100, 80, 67,78},
    {48, 69, 56, 47, 54},
    {86, 123, 100, 84, 96},
    {59, 86, 69, 58, 67},
    {87, 126, 101, 85, 100},
    {87, 125, 101, 85, 97},
    };

    return calculateScaling[outputSize][inputSize];
}

//void MoreSettingsForCopy::on_paperTypeList_currentIndexChanged(int index)
//{
//    ParamForCopy->paperType = MediaType_Copy(index);
//}

bool MoreSettingsForCopy::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->scaling)
    {
        if(event->type() == QEvent::FocusOut)
        {
            on_scaling_editingFinished();
            ui->label_tip->hide();
        }
    }
    return QWidget::eventFilter(watched,event);
}
