#ifndef SETTINGWARMING_H
#define SETTINGWARMING_H

#include <QDialog>
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>

namespace Ui {
class SettingWarming;
}

class SettingWarming : public QDialog
{
    Q_OBJECT

public:
    explicit SettingWarming(QWidget *parent = 0, QString text = "", bool showLabel = false);
    ~SettingWarming();
    Ui::SettingWarming *ui;
protected:
    void paintEvent(QPaintEvent *);

};

#endif // SETTINGWARMING_H
