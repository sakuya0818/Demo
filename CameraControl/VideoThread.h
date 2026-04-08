#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QImage>
#include <QThread>
#include <opencv2/opencv.hpp>

class VideoThread:public QThread
{
    Q_OBJECT
public:
    VideoThread();
    ~VideoThread();
    virtual void close();
    bool save(QString filePath, QString fileName, QString &picturePath);
    bool check();
    QImage matToQImage(const cv::Mat&);
protected:
    void run() override;

signals:
    void sig_GetOneFrame(QImage);
private:
    QImage m_image;
    bool m_stop;
    bool m_check;
};

#endif // VIDEOTHREAD_H
