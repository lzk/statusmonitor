#ifndef CHECKFINGERDIALOG_H
#define CHECKFINGERDIALOG_H

#include <QDialog>

namespace Ui {
class CheckFingerDialog;
}

class CheckFingerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CheckFingerDialog(QWidget *parent = 0);
    ~CheckFingerDialog();


private:
    Ui::CheckFingerDialog *ui;
};

#endif // CHECKFINGERDIALOG_H
