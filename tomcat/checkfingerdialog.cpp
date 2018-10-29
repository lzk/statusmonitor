#include "checkfingerdialog.h"
#include "ui_checkfingerdialog.h"
#include <QMovie>
#include <QDialogButtonBox>
#include <QPushButton>
CheckFingerDialog::CheckFingerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckFingerDialog)
{
    ui->setupUi(this);

//    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("");

    ui->buttonBox->removeButton(ui->buttonBox->button(QDialogButtonBox::Ok));

    QMovie *movie = new QMovie(":/image/timg.gif");
    ui->label_gif->setMovie(movie);
    movie->start();
}

CheckFingerDialog::~CheckFingerDialog()
{
    delete ui;
}
