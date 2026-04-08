#include "VideoThread.h"

#include "QMessageBox"
#include <QDateTime>
#include <QDebug>

VideoThread::VideoThread()
    :m_stop(false)
{
    m_check = true;
    qDebug() << "VideoThread: 线程已创建";
}

VideoThread::~VideoThread()
{
    qDebug() << "VideoThread: 析构函数被调用";
    close();
}

void VideoThread::close()
{
    qDebug() << "VideoThread: 设置停止标志";
    m_stop = true;
}

QImage VideoThread::matToQImage(const cv::Mat& mat)
{
    // 检查输入是否为空
    if (mat.empty())
    {
        return QImage();
    }
    // 确保是 8 位无符号整数类型
    if (mat.type() != CV_8UC3)
    {
        // 如果不是 3 通道，可根据需要转换（如灰度图转彩色）
        cv::cvtColor(mat, mat, cv::COLOR_GRAY2BGR);
    }
    // 转换颜色通道：BGR → RGB
    cv::Mat rgbMat;
    cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);

    // 创建 QImage，直接使用 rgbMat 的内存（避免拷贝）
    QImage image(
                rgbMat.data,
                rgbMat.cols,
                rgbMat.rows,
                static_cast<int>(rgbMat.step), // 每行字节数（确保内存连续）
                QImage::Format_RGB888
                );
    // 重要：因为 QImage 直接使用了 Mat 的内存，需要确保 Mat 的生命周期比 QImage 长
    // 或者返回时拷贝一份（如果 Mat 可能提前释放）
    return image.copy(); // 如果不需要共享内存，直接返回 copy 更安全
}

//通过线程进行图像解码处理
void VideoThread::run()
{
    // ========== 选择测试源 ==========
    // 方案1: RTSP流（需要摄像头和FFmpeg插件）
     std::string source = "rtsp://admin:cw123456@192.168.1.64/h264/ch33/sub/av_stream";
     int backend = cv::CAP_FFMPEG;

    // 方案2: 本地视频文件
//    std::string source = "test_video.mp4";  // 放在bin目录下
//    int backend = cv::CAP_ANY;

    // ================================

    cv::VideoCapture cap;

    // 打开视频源
    if (backend == cv::CAP_FFMPEG) {
        cap.open(source, cv::CAP_FFMPEG);
    } else {
        cap.open(source);
    }

    // 检查是否成功打开
    if (!cap.isOpened())
    {
        std::cout << "无法打开 RTSP 流" << std::endl;
        return;
    }
    cv::Mat frame, resized_img;
    QImage img;
    // 目标分辨率：1080×1920（宽×高）
    int target_width  = 1920;
    int target_height = 1080;
    while(!m_stop)
    {
        cap >> frame;
        if (frame.empty())
        {
            break;
        }
        // 缩放图像
        cv::resize(frame, resized_img, cv::Size(target_width, target_height), 0, 0, cv::INTER_LINEAR);
        img = matToQImage(resized_img);

        QImage enlarged = img.scaled(img.size() * 2, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QRect centerRect(enlarged.width() / 2 - img.width()/2,
                         enlarged.height()/ 2 - img.height()/2, img.width(), img.height());
        QImage centered = enlarged.copy(centerRect);
        emit sig_GetOneFrame(centered);

        // 添加小延迟提高线程响应性
        msleep(1);
    }
    // 释放资源
    cap.release();
    qDebug() << "VideoThread: 线程已停止，资源已释放";
}

bool VideoThread::save(QString filePath, QString fileName, QString &picturePath)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyyMMddhhmmss")+".jpg";
    fileName += str;
    if (m_image.save(filePath + "/" + fileName))
    {
        picturePath = filePath + "/" + fileName;
        return true;
    }
    return false;
}

bool VideoThread::check()
{
    return m_check;
}
