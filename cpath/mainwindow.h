#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QString selectState;
    QString unSelectState;
    QPoint last;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    bool isPress;
    void closeEvent(QCloseEvent *e);

private slots:
    void on_Copy_clicked();

    void on_Scan_clicked();

    void on_Setting_clicked();

    void on_loginButton_clicked();


    void on_CopyImgBtn_clicked();

    void on_ScanImgBtn_clicked();

    void on_SettingImgBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
