#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QLabel>
#include <QImage>

#include "VideoThread.h"

class VideoWidget : public QLabel
{
    Q_OBJECT
public:
    VideoWidget(QWidget *parent);

    ~VideoWidget();

    void Repaint(const QImage &img);

    void setOpenImage();

protected:
    void paintEvent(QPaintEvent*) override;

public slots:
    bool SaveImage();

private:
    bool m_isOpen = false;
    VideoThread* m_vt{ nullptr };
    QImage m_image;
};

#endif // VIDEOWIDGET_H
