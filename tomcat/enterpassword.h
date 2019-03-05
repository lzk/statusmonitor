#ifndef ENTERPASSWORD_H
#define ENTERPASSWORD_H

#include <QDialog>

namespace Ui {
class EnterPassword;
}

class EnterPassword : public QDialog
{
    Q_OBJECT

public:
    explicit EnterPassword(QWidget *parent = 0);
    ~EnterPassword();
    QString getPassword();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::EnterPassword *ui;
};

#endif // ENTERPASSWORD_H
