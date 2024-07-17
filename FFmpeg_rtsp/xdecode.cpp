#include "xdecode.h"
#include "qdebug.h"

xDecode::xDecode()
{

}

xDecode::xDecode(const QString url)
{
    m_url=url;
}

//采集开始前初始化
bool xDecode::begin()
{
    int  numBytes;
    avformat_network_init();   ///初始化FFmpeg网络模块，2017.8.5---lizhen
    av_register_all();         //初始化FFMPEG  调用了这个才能正常适用编码器和解码器

    //Allocate an AVFormatContext.
    pFormatCtx = avformat_alloc_context();

    ///2017.8.5---lizhen
    AVDictionary *avdic=NULL;
    char option_key[]="rtsp_transport";
    char option_value[]="tcp";
    av_dict_set(&avdic,option_key,option_value,0);
    char option_key2[]="max_delay";
    char option_value2[]="100";
    av_dict_set(&avdic,option_key2,option_value2,0);
    ///rtsp地址，可根据实际情况修改
    //char url[]="rtsp://admin:HK123456@192.168.1.64/h264/ch33/sub/av_stream";


    if (avformat_open_input(&pFormatCtx, m_url.toStdString().data(), NULL, &avdic) != 0) {
        qDebug()<<"can't open the file"<<endl;
        return false;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        qDebug()<<"Could't find stream infomation."<<endl;
        return false;
    }

    videoStream = -1;
    ///循环查找视频中包含的流信息，直到找到视频类型的流
    ///便将其记录下来 保存到videoStream变量中
    ///这里我们现在只处理视频流  音频流先不管他
    for (uint32_t i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
        }
    }

    ///如果videoStream为-1 说明没有找到视频流
    if (videoStream == -1) {
        qDebug()<<"Didn't find a video stream."<<endl;
        return false;
    }

    ///查找解码器
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    ///2017.8.9---lizhen
    pCodecCtx->bit_rate =0;   //初始化为0
    pCodecCtx->time_base.num=1;  //下面两行：一秒钟25帧
    pCodecCtx->time_base.den=10;
    pCodecCtx->frame_number=1;  //每包一个视频帧
    if (pCodec == NULL) {
        qDebug()<<"Codec not found"<<endl;
        return false;
    }
    ///打开解码器
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        qDebug()<<"Could not open codec"<<endl;
        return false;
    }

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    ///这里我们改成了 将解码后的YUV数据转换成RGB32
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                                     pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
                                     AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
    numBytes = avpicture_get_size(AV_PIX_FMT_RGB32, pCodecCtx->width,pCodecCtx->height);

    out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, out_buffer, AV_PIX_FMT_RGB32,
                   pCodecCtx->width, pCodecCtx->height);

    int y_size = pCodecCtx->width * pCodecCtx->height;
    packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
    av_new_packet(packet, y_size); //分配packet的数据
    return true;
}
//采集函数
bool xDecode::samp(QImage &img)
{
    int ret=0;
    if (av_read_frame(pFormatCtx, packet) < 0)
    {
        return false; //这里认为视频读取完了
    }
    if (packet->stream_index == videoStream) {
        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,packet);
        if (ret < 0) {
            qDebug()<<"decode error"<<endl;
            return false;
        }

        if (got_picture) {
            sws_scale(img_convert_ctx,
                      (uint8_t const * const *) pFrame->data,
                      pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                      pFrameRGB->linesize);
            //把这个RGB数据 用QImage加载
            // QImage tmpImg((uchar *)out_buffer,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);
            //tmpImg.copy(); //把图像复制一份 传递给界面显示
            //m_Image=tmpImg.copy();
            img =QImage((uchar *)out_buffer,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);

        }
    }
    av_free_packet(packet); //释放资源,否则内存会一直上升
    return true;
}
//采集结束
void xDecode::end()
{
    av_free(out_buffer);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}
