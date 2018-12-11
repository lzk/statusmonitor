#include "wlantitlecell.h"
#include "ui_wlantitlecell.h"

#define DEFWIDTH 220
#define DEFTITELHIGHT 180
#define RETRYTIMES 3;
WlanTitleCell::WlanTitleCell(QWidget *parent,  bool wlanON, bool *_islogin) :
    QStackedWidget(parent),
    ui(new Ui::WlanTitleCell)
{
    ui->setupUi(this);

    //timer1 = new QTimer(this);
    cycleCount = 0;
    currentAPID = 0;
    currentAp = new APInfo;
    isManual = false;
    //isWlanOn = wlanON;
    ui->btWLANON1->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
    ui->btWLANON2->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
    ui->btManualWiFi->hide();
    ui->btFlesh->hide();
    ui->label_line->hide();
    ui->label_network->hide();

    this->setCurrentIndex(0);
    this->resize(QSize(220,60));
    widget = new QWidget(ui->page1);
    widget->setGeometry(QRect(0,73,DEFWIDTH,10));
    currentSize.setWidth(DEFWIDTH);
    currentSize.setHeight(10);
    pageLayout.setSpacing(1);
    pageLayout.setMargin(1);

//    device->emit_cmd_plus(CMD_WIFI_refresh_plus);

//    cycle = new BusyRefreshLabel(parent);
//    cycle->setGeometry(QRect(90,130,50,50));

    //connect(timer1, SIGNAL(timeout()), this, SLOT(on_timeout()));

    QRegExp regexp1("^[\\x0020-\\x007e]{1,32}$");
    QValidator *validator1 = new QRegExpValidator(regexp1, this);
    ui->lineEdit_SSID->setValidator(validator1);

    QRegExp regexp2("^[\\x0020-\\x007e]{8,63}$");
    QValidator *validator2 = new QRegExpValidator(regexp2, this);
    ui->lineEdit_Password->setValidator(validator2);

    ui->combox_encryption->setCurrentIndex(2);
    ui->label_keyid->hide();
    ui->btKey1->hide();
    ui->btKey2->hide();
    ui->btKey3->hide();
    ui->btKey4->hide();

    ui->lineEdit_Password->setEchoMode(QLineEdit::Password);

}

WlanTitleCell::~WlanTitleCell()
{
    delete ui;
}

void WlanTitleCell::on_btManualWiFi_clicked()
{
    this->resize(DEFWIDTH, 310);
    this->setMinimumHeight(310);
    this->setCurrentIndex(1);
}

void WlanTitleCell::on_btCancel_clicked()
{
//    emit SizeChanged( QSize(211, 310), QSize(211, 71));
    this->resize(DEFWIDTH, currentSize.height() + DEFTITELHIGHT);
    widget->resize(DEFWIDTH, currentSize.height());
    this->setMinimumHeight(this->size().height());
    this->setCurrentIndex(0);
}

void WlanTitleCell::updateAP()
{
    while(!(apList.isEmpty()))
   {
       currentSize.setHeight( currentSize.height() - qobject_cast<QWidget *>(apList.last())->size().height() - 1);
       widget->resize(currentSize);
       widget->setMinimumHeight(currentSize.height());

       this->setMinimumHeight(DEFTITELHIGHT + currentSize.height());
       this->resize(QSize(DEFWIDTH, DEFTITELHIGHT + currentSize.height()));
       disconnect(qobject_cast<QWidget *>(apList.last()), SIGNAL(SizeChanged(QSize,QSize)), this, SLOT(getSizeChanged(QSize,QSize)));
       pageLayout.removeWidget(qobject_cast<QWidget *>(apList.last()));
       apList.last()->deleteLater();

       apList.removeLast();
       widget->setLayout(&pageLayout);
   }
   aList.clear();
   apList.clear();

   cycle->stopAnimation();
}

void WlanTitleCell::on_btFlesh_clicked()
{
//    timer1->start(1000);
    while(!(apList.isEmpty()))
   {
       currentSize.setHeight( currentSize.height() - qobject_cast<QWidget *>(apList.last())->size().height() - 1);
       widget->resize(currentSize);
       widget->setMinimumHeight(currentSize.height());

       this->setMinimumHeight(DEFTITELHIGHT + currentSize.height());
       this->resize(QSize(DEFWIDTH, DEFTITELHIGHT + currentSize.height()));
       disconnect(qobject_cast<QWidget *>(apList.last()), SIGNAL(SizeChanged(QSize,QSize)), this, SLOT(getSizeChanged(QSize,QSize)));
       pageLayout.removeWidget(qobject_cast<QWidget *>(apList.last()));
       apList.last()->deleteLater();

       apList.removeLast();
       widget->setLayout(&pageLayout);
   }
   aList.clear();

    cycle->startAnimation(20);

    //device->cmd_wifi_refresh();

}
