#include "VideoWidget.h"
#include "QPainter"
#include <QDebug>
#include <QPushButton>
#include <QDateTime>
#include <QApplication>

VideoWidget::VideoWidget(QWidget *parent):QLabel(parent)
{
    // 播放按钮
    QPushButton* btn_Play = new QPushButton(this);
    btn_Play->setGeometry(this->x() + 2, this->y() + 2, 30, 30);
    this->lower();
    btn_Play->raise();
    btn_Play->setStyleSheet("color:green;border-image: url(:/image/stop_white.png);");
    btn_Play->setMinimumSize(30, 30);
    btn_Play->setMaximumSize(30, 30);
    connect(btn_Play, &QPushButton::clicked, this, [=](){
        m_isOpen = !m_isOpen;
        if(!m_isOpen)
        {
            QImage image;
            Repaint(image);
        }
        else
        {
            if(!m_vt->isRunning())
            {
                m_vt->start();
            }
        }
    });

    // 拍照按钮
    QPushButton* btn_SaveImg = new QPushButton(this);
    btn_SaveImg->setGeometry(this->x()+40, this->y()+2, 30, 30);
    btn_SaveImg->raise();
    btn_SaveImg->setStyleSheet("color:green;border-image: url(:/image/takePicWhite.png);");
    btn_SaveImg->setMinimumSize(30, 30);
    btn_SaveImg->setMaximumSize(30, 30);
    connect(btn_SaveImg, &QPushButton::clicked, this, &VideoWidget::SaveImage);

    // VideoWidget 负责管理这个对象的生命周期
    m_vt = new VideoThread();
    // 连接信号槽
    connect(m_vt, &VideoThread::sig_GetOneFrame, this, [this](QImage image){
        if(m_isOpen)
        {
            Repaint(image);
        }
    });

    qDebug() << "VideoWidget: 初始化完成";
}

VideoWidget::~VideoWidget()
{
    qDebug() << "VideoWidget: 正在停止视频线程...";

    // 先设置停止标志
    if(m_vt)
    {
        m_vt->close();

        // 等待线程停止（最多3秒）
        if(m_vt->isRunning())
        {
            qDebug() << "VideoWidget: 等待线程结束...";
            if(!m_vt->wait(3000))
            {
                qDebug() << "VideoWidget: 线程未能在3秒内停止，强制终止";
                m_vt->terminate();
                m_vt->wait(1000);
            }
        }

        delete m_vt;
        m_vt = nullptr;
    }

    qDebug() << "VideoWidget: 视频线程已停止";
}

void VideoWidget::setOpenImage()
{
    if(m_isOpen)
    {
        return ;
    }
    m_isOpen = true;
    if(!m_vt->isRunning())
    {
        m_vt->start();
    }
}

void VideoWidget::Repaint(const QImage &img)
{
    if(img.isNull())
    {
        QPalette palett;
        palett.setBrush(QPalette::Background,QBrush(QPixmap::fromImage(img.scaled(QSize(this->width(),this->height())))));
        this->setPalette(palett);
        this->setAutoFillBackground(true);
        return;
    }
    QPalette palett;
    palett.setBrush(QPalette::Background,QBrush(QPixmap::fromImage(img.scaled(QSize(this->width(),this->height())))));
    this->setPalette(palett);
    this->setAutoFillBackground(true);
    m_image = img;
}

void VideoWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QColor(0, 255, 255, 100));
    painter.setBrush(QColor(0, 255, 255, 100));
    int h,w;
    w = this->width()/2;
    h = this->height()/2;
    painter.drawLine(w,   h-50, w,   h+50);
    painter.drawLine(w-60,h,    w+60,h);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
}

bool VideoWidget::SaveImage()
{
    QString filePath = QApplication::applicationDirPath();
    QDateTime time = QDateTime::currentDateTime();
    QString fileName = time.toString("yyyyMMddhhmmss")+".jpg";
    QString path = filePath + "/拍照图片/" + fileName;
    if(m_image.isNull())
    {
        qDebug()<<"图片为空";
        return false;
    }
    if (m_image.save(path))
    {
        qDebug()<<"拍照成功保存至："<<path;
        return true;
    }
    return false;
}
