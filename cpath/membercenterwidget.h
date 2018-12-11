#ifndef MEMBERCENTERWIDGET_H
#define MEMBERCENTERWIDGET_H

#include <QWidget>
#include "qpushbutton.h"
#include "qstackedwidget.h"
#include <QtNetwork/QtNetwork>

namespace Ui {
class MemberCenterWidget;
}

typedef struct UserInfo
{
    QString mobile;
    QString email;
    QString realName;
    QString address;
    QString sex;        //1:Man  0:Woman
    QString birthDate;
    QString mobileCode;
}StrUserInfo;

class MemberCenterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemberCenterWidget(QWidget *parent = 0);
    ~MemberCenterWidget();

public slots:
    void setSW(QStackedWidget* _sw, QPushButton * _bt);

private slots:
    void on_btLogin_clicked();

    void on_btAbout_clicked();

    void on_btCancel_clicked();

    void on_btChInfo_clicked();

    void on_btExpe_clicked();

    void on_pushButton_clicked();

    void getUserInfo();

    void replyFinish_get(QNetworkReply* reply);

    QString getHostMacAddress();

    void setUserInfo();

    void replyFinish_set(QNetworkReply* reply);

    void on_btApply_clicked();

private:
    Ui::MemberCenterWidget *ui;

    QStackedWidget* sw;
    QPushButton *bt;
    bool isLogin;
    QString loginPhone;
    StrUserInfo currentUser;
    QString m_strKey;
};

#endif // MEMBERCENTERWIDGET_H
