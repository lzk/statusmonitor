#include "thumbnailview.h"
#include "thumbnailimage.h"
#include <QThread>

#include <QLabel>
#include <QFile>
#include <QDebug>

#include <QDir>
#include <QFileInfo>

QString get_preview_file_name(const QString& origin_file_name)
{
#if DEBUG_VIEW
    return origin_file_name;
#else
    QFileInfo info(origin_file_name);
    return info.dir().absolutePath() +"/prvw_" + info.fileName();
#endif
}

QString get_thumbnail_file_name(const QString& origin_file_name)
{
#if DEBUG_VIEW
    return origin_file_name;
#else
    QFileInfo info(origin_file_name);
    return info.dir().absolutePath() +"/thmb_" + info.fileName();
#endif
}

ImageHandler::ImageHandler(QObject *parent)
    : QObject(parent)
{
}

ImageHandler::~ImageHandler()
{
}

void ImageHandler::image_rm(const QString &path)
{
    QFile::remove(path);
#if !DEBUG_VIEW
    QFile::remove(get_preview_file_name(path));
#endif
}

void ImageHandler::image_save(const QString& path ,int angle)
{
    angle %= 4;
    if(angle){
        QTransform transform;
        transform.rotate(90 * angle);
        QImage(path).transformed(transform).save(path);
#if !DEBUG_VIEW
        QImage(get_preview_file_name(path)).transformed(transform).save(get_preview_file_name(path));
#endif
    }
}

void ImageHandler::image_answer_then_send(QObject* obj ,QListWidgetItem* item ,QSize size ,int weight ,int angle)
{
    QImage image;
    //get source image
    QString image_path = item->data(Qt::UserRole).toString();
    QSize image_size = item->data(Qt::UserRole + 1).toSize();
    QImage prev_image = QImage(get_preview_file_name(image_path));
    if(image_size.isEmpty()){
        image_size = prev_image.size();
    }
    QSize prev_size = prev_image.size();
    if(prev_size.isEmpty() || image_size.isEmpty() || size.isEmpty()){
        emit image_send(obj ,image ,-1 ,0);
        return;
    }
    //rotate source image if angle
    if(angle % 4){
        QTransform trans;
        prev_image = prev_image.transformed(trans.rotate(90 * (angle % 4)));
        if(angle % 2){
            prev_size = prev_image.size();
            image_size = QSize(image_size.height() ,image_size.width());
        }
    }
    //get scale new size
    qreal rate = 1.0;
    qreal rate1,rate2;
    rate1 = size.width() * 1.0 / prev_size.width();
    rate2 = size.height() * 1.0 / prev_size.height();

    bool rate_fit;
    QSize new_size;

//    qDebug("before weight:%d" ,weight);
    int flag = !!weight;
    rate = rate1 > rate2 ?rate2 :rate1;
    QSize fit_size = rate * prev_size;//fit size
    do{
        rate_fit = true;
        new_size = fit_size * (1 + (0.1 * weight));
        if(weight > 0){
            if((fit_size - image_size).isValid()){
                new_size = image_size;
            }else if((fit_size * (1.1 + (0.1 * weight)) - image_size).isValid()){
                flag = 2;//max
                weight --;
                rate_fit = false;
            }
        }else if(weight < 0){
            if((fit_size * (0.9 + (0.1 * weight)) - 0.333333 * size).isEmpty()){
                flag = -2;//min
                weight ++;
                rate_fit = false;
            }
        }else{
            if((fit_size - image_size).isValid()){
                new_size = image_size;
            }
        }
    }while(!rate_fit);
    if(flag == -2){
        weight--;
    }else if(flag == 2){
        weight++;
    }
//    qDebug("after weight:%d" ,weight);
    //scale image
    QLabel label;
    label.setScaledContents(true);
    label.setPixmap(QPixmap::fromImage(prev_image));
    label.resize(new_size);
    image = label.pixmap()->toImage();

    emit image_send(obj ,image ,flag ,weight);
}

ThumbnailView::ThumbnailView(QWidget *parent) :
    QListWidget(parent)
{
    image_handler = new ImageHandler(this);
    setSpacing(5);
    setViewMode(QListView::IconMode);
    setFlow(QListView::LeftToRight);
//    setEditTriggers(QAbstractItemView::DoubleClicked);
    setSelectionMode(QAbstractItemView::MultiSelection);
    setResizeMode(QListView::Adjust);
//    setDragEnabled(false);
    setMovement(QListView::Static);
//    setMaximumHeight(300);
}

ThumbnailView::~ThumbnailView()
{
}


void ThumbnailView::add_image_item(const QString& origin_image_path ,QSize iamge_size)
{
    QListWidgetItem* item = new QListWidgetItem();
    insertItem(0 ,item);
    item->setSizeHint(QSize(width() / 3 - 15 ,height() - 10));
    item->setData(Qt::UserRole ,origin_image_path);
    item->setData(Qt::UserRole + 1 ,iamge_size);
    ThumbnailImage* widget = new ThumbnailImage(item ,image_handler);
    connect(widget ,SIGNAL(print_scan_images(QStringList)) ,this ,SIGNAL(print_scan_images(QStringList)));
    setItemWidget(item ,widget);
    //to make relayout to avoid some bug
    QSize _size = size();
    resize(10 ,10);
    resize(_size);
}

void ThumbnailView::selectionChanged(const QItemSelection & selected, const QItemSelection & )
{
    if(selectedItems().count() > 99){
        selectionModel()->select(selected ,QItemSelectionModel::Deselect);
    }
}


