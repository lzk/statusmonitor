#include "animationdlg.h"
#include "ui_animationdlg.h"

AnimationDlg::AnimationDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnimationDlg)
{
    ui->setupUi(this);
}

AnimationDlg::~AnimationDlg()
{
    delete ui;
}
