#ifndef XDECODE_H
#define XDECODE_H
#include "QString"
#include "QImage"
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
}


class xDecode
{
public:
    xDecode();
    xDecode(const QString url);
    bool begin(); //初始化
    bool samp(QImage &img); //采集函数
    void end();  //结束
private:
    QString m_url;
    AVFormatContext *pFormatCtx;
    AVPacket *packet;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVFrame *pFrame, *pFrameRGB;
    struct SwsContext *img_convert_ctx;
    int videoStream, got_picture;
    uint8_t *out_buffer;
};

#endif // XDECODE_H
