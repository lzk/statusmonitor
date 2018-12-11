#ifndef WIFISETTINGWEPCELL_H
#define WIFISETTINGWEPCELL_H

#include <QStackedWidget>

namespace Ui {
class WiFiSettingWEPCell;
}

class WiFiSettingWEPCell : public QStackedWidget
{
    Q_OBJECT

public:
    explicit WiFiSettingWEPCell(QWidget *parent = 0);
    ~WiFiSettingWEPCell();

private:
    Ui::WiFiSettingWEPCell *ui;
};

#endif // WIFISETTINGWEPCELL_H
