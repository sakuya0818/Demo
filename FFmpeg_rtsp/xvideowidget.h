#ifndef XVIDEOWIDGET_H
#define XVIDEOWIDGET_H

#include <QLabel>
#include <QImage>


class IVideoCall
{
public:
    virtual void Repaint(const QImage &img)=0;
};

class XVideoWidget:public QLabel,public IVideoCall
{
    Q_OBJECT
public:
    XVideoWidget(QWidget *parent);
    virtual void Repaint(const QImage &img) override;
protected:


};

#endif // XVIDEOWIDGET_H
