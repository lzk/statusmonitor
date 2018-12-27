#include "moresettingsforcopy.h"
#include "ui_moresettingsforcopy.h"
#include <QStandardItemModel>
#include <QListView>
#include <QPalette>

#define MIN_SCALING 25
#define MAX_SCALING 400

MoreSettingsForCopy::MoreSettingsForCopy(QWidget *parent, bool idCardFlag, Param_Copy *pParam) :
    QDialog(parent),
    ui(new Ui::MoreSettingsForCopy)
{
    timer = new QTimer(this);
    scaling = 0;
    _idCardFlag = idCardFlag;

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


    if(!_idCardFlag)
    {
        showParam();
    }else
    {
        ParamForCopy->scaling = 100;
//        ParamForCopy->docType = DocType_Copy_Photo;
        ParamForCopy->docDpi = DocDpi_Copy_DPI600;
        ParamForCopy->outputSize = OutPutSize_Copy_A4;
        ParamForCopy->isMultiPage = false;
        ParamForCopy->multiMode = TwoInOne;
        showParam();
    }

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
    if(!_idCardFlag)
    {
        ParamForCopy->scaling = (int)p->scale;
        ParamForCopy->docType = (DocType_Copy)p->scanMode;
        ParamForCopy->docSize = (DocSize_Copy)p->orgSize;
        ParamForCopy->docDpi = (DocDpi_Copy)p->dpi;
        ParamForCopy->outputSize = (OutPutSize_Copy)p->paperSize;
        ParamForCopy->paperType = (MediaType_Copy)p->mediaType;
        ParamForCopy->isMultiPage = false;
        ParamForCopy->multiMode = (MultiMode_Copy)p->nUp;

        ParamForCopy->promptInfo.isIDCard = true;
        ParamForCopy->promptInfo.isMultible = true;

        showParam();
    }else
    {
        ParamForCopy->docType = (DocType_Copy)p->scanMode;
        ParamForCopy->paperType = (MediaType_Copy)p->mediaType;

        ParamForCopy->promptInfo.isIDCard = true;
        ParamForCopy->promptInfo.isMultible = true;
        showParam();
    }

}

void MoreSettingsForCopy::showParam()
{
    QString text;
    if(!_idCardFlag)
    {
        ui->scaling->setText(text.setNum(ParamForCopy->scaling));

        if(ParamForCopy->docType == DocType_Copy_Photo)
        {
            ui->btPicture->setChecked(true);
            ui->btText->setChecked(false);
        }else
        {
            ui->btPicture->setChecked(false);
            ui->btText->setChecked(true);
        }
        ui->docSizeList->setCurrentIndex(ParamForCopy->docSize);
        ui->dpiList->setCurrentIndex(ParamForCopy->docDpi);
        ui->outPutSizeList->setCurrentIndex(ParamForCopy->outputSize);
        ui->paperTypeList->setCurrentIndex(ParamForCopy->paperType);

        if(ParamForCopy->isMultiPage)
        {
             ui->isNinOne->setChecked(true);
             selectMode(ParamForCopy->multiMode);

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

        ParamForCopy->promptInfo.isIDCard = !ParamForCopy->promptInfo.isIDCard;
        on_btID_clicked();  //ParamForCopy->promptInfo.isIDCard = true;
        ParamForCopy->promptInfo.isMultible = !ParamForCopy->promptInfo.isMultible;
        on_btNInOne_clicked();  //ParamForCopy->promptInfo.isMultible
    }else
    {
        ui->label_1->setDisabled(true);
        ui->label_3->setDisabled(true);
        //ui->label_7->setDisabled(true);
        ui->isNinOne->setDisabled(true);

        ui->btReduce->setDisabled(true);
        ui->btAdd->setDisabled(true);
        ui->label->setDisabled(true);


        ui->scaling->setText(text.setNum(ParamForCopy->scaling));
        ui->scaling->setDisabled(true);

        if(ParamForCopy->docType == DocType_Copy_Photo)
        {
            ui->btPicture->setChecked(true);
            ui->btText->setChecked(false);
        }else
        {
            ui->btPicture->setChecked(false);
            ui->btText->setChecked(true);
        }

        ui->docSizeList->setCurrentIndex(ParamForCopy->docSize);
        ui->docSizeList->setDisabled(true);
        ui->dpiList->setCurrentIndex(ParamForCopy->docDpi);
        ui->outPutSizeList->setCurrentIndex(ParamForCopy->outputSize);
        ui->paperTypeList->setCurrentIndex(ParamForCopy->paperType);
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
         if(model2) model2->item(0)->setEnabled(false);

        if(ParamForCopy->isMultiPage)
        {
             ui->isNinOne->setChecked(true);
             selectMode(ParamForCopy->multiMode);
        }
        else
        {
            ui->isNinOne->setChecked(false);
        }

        ui->isNinOne->setDisabled(true);

        ParamForCopy->promptInfo.isIDCard = !ParamForCopy->promptInfo.isIDCard;
        on_btID_clicked();  //ParamForCopy->promptInfo.isIDCard = true;
        ParamForCopy->promptInfo.isMultible = !ParamForCopy->promptInfo.isMultible;
        on_btNInOne_clicked();  //ParamForCopy->promptInfo.isMultible
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
        ParamForCopy->scaling = tmp - 1;
        ui->scaling->setText(text.setNum(ParamForCopy->scaling));
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

    if((tmp >= MIN_SCALING)&&(tmp < MAX_SCALING) && (!ParamForCopy->isMultiPage))
    {
        ui->label_tip->hide();
        ParamForCopy->scaling = tmp + 1;
        ui->scaling->setText(text.setNum(ParamForCopy->scaling));
    }else
    {
        if(!ParamForCopy->isMultiPage)
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
        ParamForCopy->scaling = 100;
        ui->scaling->setStyleSheet("#scaling {border:transparent;}");
        ui->btReduce->setEnabled(true);
        ui->btAdd->setEnabled(true);
        ui->btOK->setEnabled(true);
    }
    else{
        ParamForCopy->scaling = tmp;
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
        ui->label_30->setEnabled(true);
        ui->label_31->setEnabled(true);
        ui->label_32->setEnabled(true);
        ui->scaling->setDisabled(true);
        ui->label_1->setDisabled(true);
        ui->label->setDisabled(true);

        ParamForCopy->isMultiPage = true;

       switch (ParamForCopy->multiMode)
       {
           case TwoInOne: ui->label_20->setStyleSheet("#label_20 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
           case FourInOne: ui->label_21->setStyleSheet("#label_21 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
           case NineInOne: ui->label_22->setStyleSheet("#label_22 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
           default:break;
       }

       model->item(3)->setEnabled(false);
       model->item(5)->setEnabled(false);
    }
    else{
        ui->label_30->setDisabled(true);
        ui->label_31->setDisabled(true);
        ui->label_32->setDisabled(true);
        ui->scaling->setEnabled(true);
        ui->label_1->setEnabled(true);
        ui->label->setEnabled(true);

        ParamForCopy->isMultiPage = false;

        switch (ParamForCopy->multiMode)
        {
            case TwoInOne: ui->label_20->setStyleSheet("#label_20 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
            case FourInOne: ui->label_21->setStyleSheet("#label_21 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
            case NineInOne: ui->label_22->setStyleSheet("#label_22 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
            default:break;
        }
        model->item(3)->setEnabled(true);
        model->item(5)->setEnabled(true);
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
    switch (ParamForCopy->multiMode)
    {
        case TwoInOne: ui->label_20->setStyleSheet("#label_20 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
        case FourInOne: ui->label_21->setStyleSheet("#label_21 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
        case NineInOne: ui->label_22->setStyleSheet("#label_22 {background-color: rgb(198, 198, 198);border-radius:8px;}"); break;
        default:break;
    }

    ParamForCopy->multiMode = mode;

    switch (ParamForCopy->multiMode)
    {
        case TwoInOne:  ui->label_20->setStyleSheet("#label_20 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
        case FourInOne: ui->label_21->setStyleSheet("#label_21 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
        case NineInOne: ui->label_22->setStyleSheet("#label_22 {background-color: rgb(157, 157, 157);border-radius:8px;}"); break;
        default:break;
    }
}

void MoreSettingsForCopy::on_bt2in1_clicked()
{
    if(ParamForCopy->isMultiPage)    selectMode(TwoInOne);
}

void MoreSettingsForCopy::on_bt4in1_clicked()
{
    if(ParamForCopy->isMultiPage)    selectMode(FourInOne);
//     ui->label_30->setText("here");
}

void MoreSettingsForCopy::on_bt9in1_clicked()
{
    if(ParamForCopy->isMultiPage)    selectMode(NineInOne);
}

void MoreSettingsForCopy::on_btID_clicked()
{
    ParamForCopy->promptInfo.isIDCard = !ParamForCopy->promptInfo.isIDCard;
    if(ParamForCopy->promptInfo.isIDCard)
    {
        ui->btID->setStyleSheet("#btID {border-image: url(:/Images/CheckBox_Open.png);}");
    }
    else
    {
        ui->btID->setStyleSheet("#btID {border-image: url(:/Images/CheckBox_Close.png);}");
    }
}

void MoreSettingsForCopy::on_btNInOne_clicked()
{
    ParamForCopy->promptInfo.isMultible = !ParamForCopy->promptInfo.isMultible;
    if(ParamForCopy->promptInfo.isMultible)
    {
        ui->btNInOne->setStyleSheet("#btNInOne {border-image: url(:/Images/CheckBox_Open.png);}");
    }
    else
    {
        ui->btNInOne->setStyleSheet("#btNInOne {border-image: url(:/Images/CheckBox_Close.png);}");
    }
}

void MoreSettingsForCopy::on_btText_toggled(bool checked)
{
    if(checked)
    {
        ParamForCopy->docType = DocType_Copy_Text;
    }
}

void MoreSettingsForCopy::on_btPicture_toggled(bool checked)
{
    if(checked)
    {
        ParamForCopy->docType = DocType_Copy_Photo;
    }
}

void MoreSettingsForCopy::on_docSizeList_currentIndexChanged(int index)
{
    ParamForCopy->docSize = DocSize_Copy(index);
}

void MoreSettingsForCopy::on_dpiList_currentIndexChanged(int index)
{
    if(_idCardFlag)
    {
         ParamForCopy->docDpi = DocDpi_Copy_DPI600;
         ui->dpiList->setCurrentIndex(1);
    }else
    {
        ParamForCopy->docDpi = DocDpi_Copy(index);
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
        }
    }
    ParamForCopy->outputSize = OutPutSize_Copy(index);
}

void MoreSettingsForCopy::on_paperTypeList_currentIndexChanged(int index)
{
    ParamForCopy->paperType = MediaType_Copy(index);
}

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
