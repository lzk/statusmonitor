#ifndef IPV6STATUS_H
#define IPV6STATUS_H

#include <QDialog>

namespace Ui {
class IPv6Status;
}

class IPv6Status : public QDialog
{
    Q_OBJECT

public:
    explicit IPv6Status(QWidget *parent = 0);
    ~IPv6Status();

private:
    Ui::IPv6Status *ui;
};

#endif // IPV6STATUS_H
