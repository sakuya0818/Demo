#include "xvideowidget.h"

XVideoWidget::XVideoWidget(QWidget *parent):QLabel(parent)
{

}

void XVideoWidget::Repaint(const QImage &img)
{
     this->setPixmap(QPixmap::fromImage(img));
}

