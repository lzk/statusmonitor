#include "animationdlg.h"
#include "ui_animationdlg.h"
#define Jam "01"
#define JamExit "02"
#define OutOfPaper "03"
#define _IDCardScan "05"
#define _Nin1Copy   "06"

AnimationDlg::AnimationDlg(QWidget *parent, int status, bool *enNext) :
    QDialog(parent),
    ui(new Ui::AnimationDlg),
    enNextShow(enNext)
{
    ui->setupUi(this);
    connect(ui->btOK,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->btCancel,SIGNAL(clicked()),this,SLOT(reject()));
    this->resize(QSize(640,800));
    this->setWindowFlags(Qt::Dialog);
    label = new QLabel(this);
    label->setContentsMargins(0,0,0,0);
    label->setGeometry(0, 0, 640, 640);
    label->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setPointSize(30);
    label->setFont(font);

    map["ar_SA"] = "Arabic";
    map["bn_BG"] = "Bengali";
    map["pt_PT"] = "BarzilianPortuguse";
    map["pt_BR"] = "BarzilianPortuguse";
    map["ca_ES"] = "Catalan";
    map["cs_CZ"] = "Czech";
    map["da_DK"] = "Danish";
    map["nl_NL"] = "Dutch";
    map["en_US"] = "English";
    map["fi_FI"] = "Finnish";
    map["fr_FR"] = "French";
    map["de_DE"] = "German";
    map["el_GR"] = "Greek";
    map["hi_IN"] = "Hindi";      //北印度的 印度
    map["hu_HU"] = "Hungarian";
    map["id_ID"] = "Indonesia";  //印尼
    map["it_IT"] = "Italian";
    map["ja_JP"] = "Japanese";
    map["no_NO"] = "Norwegian";  //挪威
    map["pl_PL"] = "Polish";
    map["pt_PT"] = "Portuguese";
    map["ru_RU"] = "Russian";
    map["zh_CN"] = "SimplifiedChinese";
    map["es_ES"] = "Spanish";
    map["sv_SE"] = "Swedish";
    map["th_TH"] = "Thai";
    map["zh_TW"] = "TraditionalChinese";
    map["tr_TR"] = "Turkish";
    map["ur_PK"] = "Urdu";      //巴基斯坦的乌尔都语？
    map["vi_VN"] = "Vietnamese";

    QString lan = QLocale::system().name();                     //get the system local language
    QString dirStr = QApplication::applicationDirPath() +"/Video/"+map[lan];
//    qDebug()<<"dirStr"<<dirStr;
    videoDir =new QDir(dirStr);

    QString flag;
    switch(status)
    {
    case 1:
        flag = _IDCardScan;
        this->setWindowTitle(tr("ResStr_ID_Card_Copy"));
        break;
    case 2:
        flag = _Nin1Copy;
        this->setWindowTitle(tr("ResStr_N_in_1_Copy"));
        break;
    case 0xBD:
        flag = OutOfPaper;
        this->setWindowTitle(tr("ResStr_Out_of_Paper"));
        hideLabel();
        break;
    case 0xBC:          //PSTATUS_InitializeJam
    case 0xBE:          //PSTATUS_JamAtRegistStayOn
        flag = Jam;
        this->setWindowTitle(tr("ResStr_Jam_front"));
        hideLabel();
        break;
    case 0xBF:          //PSTATUS_JamAtExitNotReach
    case 0xC0:          //PSTATUS_JamAtExitStayOn
        flag = JamExit;
        this->setWindowTitle(tr("ResStr_Jam_back"));
        hideLabel();
        break;
    default:
        flag = Jam;
        break;
    }
    QStringList filters;
    filters << flag +"*.gif" ;
    videoDir->setNameFilters(filters);
    files = videoDir->entryInfoList();
    fquantity = files.size();
    if(fquantity > 0)
    {
        movie = new QMovie(files.at(0).filePath());
        label->setMovie(movie);
        movie->setPaused(false);
        movie->start();
        ui->bt_back->setStyleSheet("QPushButton{border-image: url(:/Images/AnimationControl/leftDisable.tif);}");
        ui->bt_back->setDisabled(true);
        index = 0;
        isPause = false;
        connect(movie, SIGNAL(finished()), this, SLOT(getFinished()));
        //add 03-16
        connect(movie, SIGNAL(finished()), this, SLOT(on_bt_next_clicked()));
    }
    else
    {
        label->setText(tr("Did not find the video!"));
        ui->bt_back->setDisabled(true);
        ui->bt_back->setStyleSheet("QPushButton{border-image: url(:/Images/AnimationControl/leftDisable.tif);}");
        ui->bt_next->setDisabled(true);
        ui->bt_next->setStyleSheet("QPushButton{border-image: url(:/Images/AnimationControl/rightDisable.tif);}");
        ui->bt_pause_play->setDisabled(true);
    }
    ui->bt_back->hide();
    ui->bt_next->hide();
}


AnimationDlg::~AnimationDlg()
{
    delete ui;
}

void AnimationDlg::getFinished()
{
    on_bt_pause_play_clicked();
}

void AnimationDlg::hideLabel()
{
    this->resize(QSize(640, 740));
    ui->label_msg1->hide();
    ui->label_msg2->hide();
    ui->btOK->hide();
    ui->btCancel->hide();
    ui->radioButton->hide();
    ui->bt_pause_play->setGeometry(QRect(300, 680, 40, 40));
}

void AnimationDlg::on_bt_pause_play_clicked()
{
    if(!isPause)
    {
        ui->bt_pause_play->setStyleSheet("QPushButton{border-image: url(:/Images/AnimationControl/playNormal.tif);}\
                                         QPushButton::hover{border-image: url(:/Images/AnimationControl/playHover.tif);}\
                                         QPushButton::Pressed{border-image: url(:/Images/AnimationControl/playPress.tif);}");
        movie->setPaused(true);
        isPause = !isPause;
    }
    else
    {
        ui->bt_pause_play->setStyleSheet(" QPushButton{border-image: url(:/Images/AnimationControl/pauseNormal.tif);}\
                                         QPushButton::hover{border-image: url(:/Images/AnimationControl/pauseHover.tif);}\
                                         QPushButton::Pressed{border-image: url(:/Images/AnimationControl/pausePress.tif);}");
        movie->start();
        isPause = !isPause;
    }
}

void AnimationDlg::on_bt_next_clicked()
{
    if(index < fquantity - 1)
    {
        index++;
        movie->stop();
        movie->setFileName(files.at(index).filePath());
        label->setMovie(movie);
        label->update();
        movie->start();
        isPause = true;
        on_bt_pause_play_clicked();
    }
    //add this for auto cycle play;
    if(!ui->bt_next->isEnabled())
    {
        index = 0;
        movie->stop();
        movie->setFileName(files.at(index).filePath());
        label->setMovie(movie);
        label->update();
        movie->start();
        isPause = true;
        on_bt_pause_play_clicked();
    }
    if(index == fquantity - 1)
    {
        ui->bt_next->setDisabled(true);
        ui->bt_next->setStyleSheet("QPushButton{border-image: url(:/Images/AnimationControl/rightDisable.tif);}");
    }
    if(index > 0)
    {
        ui->bt_back->setEnabled(true);
        ui->bt_back->setStyleSheet(" QPushButton{border-image: url(:/Images/AnimationControl/leftNormal.tif);}\
                                         QPushButton::hover{border-image: url(:/Images/AnimationControl/leftHover.tif);}\
                                         QPushButton::Pressed{border-image: url(:/Images/AnimationControl/leftPress.tif);}");
    }
}

void AnimationDlg::on_bt_back_clicked()
{
    if(index>0)
    {
        index--;
        movie->stop();
        movie->setFileName(files.at(index).filePath());
        label->setMovie(movie);
        label->update();
        movie->start();
        isPause = true;
        on_bt_pause_play_clicked();
    }
    if(index == 0)
    {
        ui->bt_back->setDisabled(true);
        ui->bt_back->setStyleSheet("QPushButton{border-image: url(:/Images/AnimationControl/leftDisable.tif);}");
    }
    if(index < fquantity - 1)
    {
        ui->bt_next->setEnabled(true);
        ui->bt_next->setStyleSheet(" QPushButton{border-image: url(:/Images/AnimationControl/rightNormal.tif);}\
                                         QPushButton::hover{border-image: url(:/Images/AnimationControl/rightHover.tif);}\
                                         QPushButton::Pressed{border-image: url(:/Images/AnimationControl/rightPress.tif);}");
    }
}

void AnimationDlg::on_radioButton_toggled(bool checked)
{
    if(checked)
        *enNextShow = false;
    else
        *enNextShow = true;
}

//void AnimationDlg::on_btCancel_clicked()
//{

//    close();
//}

//void AnimationDlg::on_btOK_clicked()
//{
//    if(enNextShow != NULL)
//        if(ui->radioButton->isChecked())        *enNextShow = true;
//    close();
//}