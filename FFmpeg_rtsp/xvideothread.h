#ifndef XVIDEOTHREAD_H
#define XVIDEOTHREAD_H
#include <xvideowidget.h>
#include <QThread>
#include <xdecode.h>

class xVideoThread:public QThread
{
public:
    xVideoThread();
    virtual void open(const QString url,IVideoCall *call);
    virtual void close();
protected:
    void run() override;

private:
    xDecode *m_decode;
    IVideoCall * m_call;
    QImage m_image;

};

#endif // XVIDEOTHREAD_H
