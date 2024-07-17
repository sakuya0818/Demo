#include "xvideothread.h"
#include "QMessageBox"
xVideoThread::xVideoThread()
{

}
//根据流地址打开图像，显示在界面控件
void xVideoThread::open(const QString url, IVideoCall *call)
{
    if(!url.isEmpty()&&call !=nullptr)
    {
        m_decode=new xDecode(url);
        this->start();
        m_call=call;
    }else{
        return;
    }
}

void xVideoThread::close()
{

}
//通过线程进行图像解码处理
void xVideoThread::run()
{
    if(nullptr==m_decode)
        return;
   if(false== m_decode->begin())
   {
//        QMessageBox::warning(nullptr,"警告","设备初始化失败");
        return;
   }
    while(1)
    {
       if(true== m_decode->samp(m_image)){
        m_call->Repaint(m_image);
        msleep(2);
       }else{
           break;
       }
    }
    m_decode->end();
}
