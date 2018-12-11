#ifndef AUTHENTICATIONDLG_H
#define AUTHENTICATIONDLG_H

#include <QDialog>
#include "busyrefreshlabel.h"

namespace Ui {
class AuthenticationDlg;
}

class AuthenticationDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AuthenticationDlg(QWidget *parent = 0);
    ~AuthenticationDlg();

private:
    Ui::AuthenticationDlg *ui;
};

#endif // AUTHENTICATIONDLG_H
