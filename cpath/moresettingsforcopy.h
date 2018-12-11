#ifndef MORESETTINGSFORCOPY_H
#define MORESETTINGSFORCOPY_H

#include <QDialog>
#include <QMouseEvent>
#include <qdebug.h>
#include <QtCore/qglobal.h>
#include <QTimer>
#include <qpushbutton.h>

namespace Ui {
class MoreSettingsForCopy;
}

enum DocType_Copy {DocType_Copy_Photo=0, DocType_Copy_Text};
enum DocSize_Copy {DocSize_Copy_A4=0, DocSize_Copy_A5, DocSize_Copy_B5, DocSize_Copy_Letter, DocSize_Copy_Executive};
enum DocDpi_Copy {DocDpi_Copy_DPI300=0, DocDpi_Copy_DPI600};
enum OutPutSize_Copy {OutPutSize_Copy_letter=0, OutPutSize_Copy_A4, OutPutSize_Copy_A5, OutPutSize_Copy_A6, OutPutSize_Copy_B5, OutPutSize_Copy_B6, OutPutSize_Copy_Executive, OutPutSize_Copy_16K};
enum MediaType_Copy {MediaType_Copy_Plain=0, MediaType_Copy_Recycled, MediaType_Copy_Thick, MediaType_Copy_Thin, MediaType_Copy_Label};
enum MultiMode_Copy {OneInOne=0,TwoInOne,FourInOne, NineInOne};

struct Prompt {
    bool isIDCard;
    bool isMultible;
};

struct Param_Copy {

    int scaling;
    DocType_Copy docType;          //doctype
    DocSize_Copy docSize;          //docsize
    DocDpi_Copy docDpi;            //docDpi
    OutPutSize_Copy outputSize;    //outputSize
    MediaType_Copy paperType;      //paperType
    bool isMultiPage;         //multi paper In One
    MultiMode_Copy multiMode;

    // infor support : bool idCard; bool nInOne;

    Prompt promptInfo;

};

class MoreSettingsForCopy : public QDialog
{
    Q_OBJECT

public:
    explicit MoreSettingsForCopy(QWidget *parent = 0, bool idCardFlay = false, Param_Copy *pParam = NULL);
    ~MoreSettingsForCopy();

private:
    Ui::MoreSettingsForCopy *ui;

    int scaling;
    bool _idCardFlag;
    Param_Copy defParam;
    Param_Copy *ParamForCopy;

    QPoint last;
    QTimer *timer;
    int timeCount;


protected:
    bool isPress;
    void selectMode(MultiMode_Copy mode);
    void setDefault();
    void showParam();

public slots:
    bool eventFilter(QObject *,QEvent *);
private slots:
    void on_btOK_clicked();
    void on_btDefault_clicked();

    void on_btReduce_clicked();
    void on_btAdd_clicked();


    void on_scaling_editingFinished();
    void on_timeout_add();
    void on_timeout_reduce();

    void on_isNinOne_toggled(bool checked);
    void on_bt2in1_clicked();
    void on_bt4in1_clicked();
    void on_bt9in1_clicked();
    void on_btID_clicked();
    void on_btNInOne_clicked();
    void on_btText_toggled(bool checked);
    void on_btPicture_toggled(bool checked);
    void on_docSizeList_currentIndexChanged(int index);
    void on_dpiList_currentIndexChanged(int index);
    void on_outPutSizeList_currentIndexChanged(int index);
    void on_paperTypeList_currentIndexChanged(int index);
    void on_btReduce_pressed();
    void on_btReduce_released();
    void on_btAdd_pressed();
    void on_btAdd_released();
    void on_scaling_textEdited(const QString &arg1);
};

#endif // MORESETTINGSFORCOPY_H
