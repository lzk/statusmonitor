#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>

namespace Ui {
class ChangePassword;
}
class ChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePassword(const QString& pass = QString() ,QWidget *parent = 0);
    ~ChangePassword();
    QString getPassword();

private slots:


    void on_lineEdit_old_textEdited(const QString &arg1);

private:
    Ui::ChangePassword *ui;

    QString password;
};

#endif // CHANGEPASSWORD_H
