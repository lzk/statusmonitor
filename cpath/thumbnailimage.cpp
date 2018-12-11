#include "thumbnailimage.h"
#include "ui_thumbnailimage.h"

#include <QListWidget>
#include <QListWidgetItem>
#include "imagepreviewdialog.h"
#include "thumbnailview.h"
#include <QBitmap>

ThumbnailImage::~ThumbnailImage()
{
    QString path = item->data(Qt::UserRole).toString();
    image_rm(path);
    delete ui;
    delete item;
}

ThumbnailImage::ThumbnailImage(QListWidgetItem* _item ,ImageHandler* handler) :
    ui(new Ui::ThumbnailImage),
    item(_item),
    image_handler(handler),
    image(QImage())
{
    ui->setupUi(this);
    pixmap = QPixmap(":/Images/close.png");
    ui->label->resize(pixmap.size());
    ui->label->setPixmap(pixmap);
//    ui->label->setMask(pixmap.mask());
    ui->label->setMask(QRegion(ui->label->rect() ,QRegion::Ellipse));
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->installEventFilter(this);

    connect(item->listWidget() ,SIGNAL(itemSelectionChanged()) ,this ,SLOT(listWidget_itemSelectionChanged()));

    connect(image_handler ,SIGNAL(image_send(QObject*,QImage,int,int)) ,this ,SLOT(image_update(QObject*,QImage)));
    connect(this ,SIGNAL(image_ask(QObject*,QListWidgetItem*,QSize,int,int))
            ,image_handler ,SLOT(image_answer_then_send(QObject*,QListWidgetItem*,QSize,int,int)));
    connect(this ,SIGNAL(image_rm(QString)),image_handler ,SLOT(image_rm(QString)));
    connect(this ,SIGNAL(image_save(QString,int)),image_handler ,SLOT(image_save(QString,int)));


    listWidget_itemSelectionChanged();
    image_ask();
}

void ThumbnailImage::listWidget_itemSelectionChanged()
{
    if(item->isSelected()){
        int index = item->listWidget()->selectedItems().indexOf(item) + 1;
//        ui->label->setText(tr("<h2><i><font color=white>%1</font></i></h2>").arg(index));
        ui->label->setText(tr("<font fontsize=15 color=white>%1</font>").arg(index));
        ui->label->setStyleSheet("background-color: green;");
    }else{
        ui->label->setPixmap(pixmap);
        ui->label->setStyleSheet("background-color: white;");
    }

}

void ThumbnailImage::image_ask()
{
    emit image_ask(this ,item ,item->sizeHint() ,0 ,0);
}

void ThumbnailImage::image_update(QObject *obj, const QImage &_image)
{
    if(obj == this){
        image = _image;
        update();
    }
}

#include <QPainter>
void ThumbnailImage::paintEvent(QPaintEvent *e)
{
    if(!image.isNull()){
        QPainter painter(this);
        painter.drawImage(QPoint((width() - image.width())/2 ,(height() - image.height()) / 2) ,image);
//    painter.drawPixmap(0,0,width(),height(),QPixmap::fromImage(image));
    }
    QWidget::paintEvent(e);
}

#include <QMessageBox>
#include <QMouseEvent>
bool ThumbnailImage::eventFilter(QObject * obj, QEvent * event)
{
    if(obj == ui->label){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            if(Qt::LeftButton == me->button()){
                if(!item->isSelected()){
                    if(QMessageBox::Yes == QMessageBox::question(NULL ,"Lenovo" ,"delete?")){
                        item->listWidget()->removeItemWidget(item);
                        item->listWidget()->takeItem(item->listWidget()->row(item));
                    }
                }
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj,event);
}

void ThumbnailImage::mouseDoubleClickEvent(QMouseEvent *)
{
    if(item){
        ImagePreviewDialog dialog(item ,image_handler);
        int ret = dialog.exec();
        int angle = ret % 4;
        //rotato
        QString path = item->data(Qt::UserRole).toString();

        if(angle){
            emit image_save(path ,angle);
            image_ask();
        }
        //print
        if(ret >= 20){
            QStringList sl;
            sl << path;
            emit print_scan_images(sl);
        }
    }
}

