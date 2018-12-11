#ifndef ANIMATIONDLG_H
#define ANIMATIONDLG_H

#include <QDialog>

namespace Ui {
class AnimationDlg;
}

class AnimationDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AnimationDlg(QWidget *parent = 0);
    ~AnimationDlg();

private:
    Ui::AnimationDlg *ui;
};

#endif // ANIMATIONDLG_H
