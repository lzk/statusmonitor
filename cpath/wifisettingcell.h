#ifndef WIFISETTINGCELL_H
#define WIFISETTINGCELL_H

#include <QStackedWidget>

namespace Ui {
class WiFiSettingCell;
}

class WiFiSettingCell : public QStackedWidget
{
    Q_OBJECT

public:
    explicit WiFiSettingCell(QWidget *parent = 0);
    ~WiFiSettingCell();

private:
    Ui::WiFiSettingCell *ui;
};

#endif // WIFISETTINGCELL_H
