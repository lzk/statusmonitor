#ifndef EXPERIENCEPRO_H
#define EXPERIENCEPRO_H

#include <QDialog>

namespace Ui {
class ExperiencePro;
}

class ExperiencePro : public QDialog
{
    Q_OBJECT

public:
    explicit ExperiencePro(QWidget *parent = 0);
    ~ExperiencePro();

private slots:
    void on_bt_OK_clicked();

private:
    Ui::ExperiencePro *ui;
};

#endif // EXPERIENCEPRO_H
