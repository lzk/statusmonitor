#ifndef SETTINGFORIPV6_H
#define SETTINGFORIPV6_H

#include <QDialog>

namespace Ui {
class SettingForIPv6;
}

class SettingForIPv6 : public QDialog
{
    Q_OBJECT

public:
    explicit SettingForIPv6(QWidget *parent = 0);
    ~SettingForIPv6();

private:
    Ui::SettingForIPv6 *ui;
};

#endif // SETTINGFORIPV6_H
