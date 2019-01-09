//
// Created by Administrator on 2019/1/4.
//


#include "VideoChannel.h"



void *task_decode(void *args){
    VideoChannel *videoChannel = static_cast<VideoChannel *>(args);
    videoChannel->decode();
    return 0;
}
void *task_render(void *args){
    VideoChannel *videoChannel = static_cast<VideoChannel *>(args);
    videoChannel->render();
    return 0;
}


/**
 * 1、解码
 * 2、播放
 * @param id
 */
VideoChannel::VideoChannel(int id,AVCodecContext *avCodecContext) :BaseChannel(id,avCodecContext){
    frames.setReleaseCallback(releaseFrame);
}


VideoChannel::~VideoChannel() {
    frames.clear();
}

void VideoChannel::play() {
    isPlaying= 1;
    //解码
    pthread_create(&pid_decode,0,task_decode,this);
    //渲染播放
    pthread_create(&pid_render,0,task_render,this);
}
//解码
void VideoChannel::decode() {
    AVPacket *avPacket = 0;
    while(isPlaying){
        //取出一个数据包
        int ret =packets.pop(avPacket);
        if (!isPlaying){
            break;
        }
        //取出失败
        if (!ret){
            continue;
        }
        //包丢给解码器
        ret = avcodec_send_packet(avCodecContext,avPacket);
        releasePacket(&avPacket);
        //重试
        if (ret !=0){
            break;
        }
        //代表一个图像，将这个图像输出来
        AVFrame *frame =av_frame_alloc();
        //从解码器读取 解码后的数据位 AVFrame
        ret = avcodec_receive_frame(avCodecContext,frame);
        //需要更多的数据才能够进行解码
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if(ret != 0){
            break;
        }
        frames.push(frame);
    }
    releasePacket(&avPacket);
}
//播放 保证视频流畅度
void VideoChannel::render() {

    //目标： RGBA
    swsContext = sws_getContext(
            avCodecContext->width,
            avCodecContext->height,
            avCodecContext->pix_fmt,
            avCodecContext->width,
            avCodecContext->height,
            AV_PIX_FMT_RGBA,
            SWS_BILINEAR,
            0, 0, 0);

    AVFrame *avFrame = 0;
    //指针数组
    uint8_t *dst_data[4];
    int dst_linesize[4];
    av_image_alloc(dst_data,
                   dst_linesize,
                   avCodecContext->width,
                   avCodecContext->height,
                   AV_PIX_FMT_RGBA,
                   1);
    while(isPlaying){
        int ret = frames.pop(avFrame);
        if (!isPlaying){
            break;
        }
        //src_linesize:表示每一行存放 字节长度
        sws_scale(swsContext,
                  reinterpret_cast<const uint8_t *const *>(avFrame->data),
                  avFrame->linesize,
                  0,
                  avCodecContext->height,
                  dst_data,
                  dst_linesize
        );
        //回调出去进行播放 TODO
        renderFrameCallback(dst_data[0], dst_linesize[0], avCodecContext->width, avCodecContext->height);
        //释放Frame
        releaseFrame(&avFrame);
    }

    av_freep(&dst_data[0]);
    releaseFrame(&avFrame);
}
void VideoChannel::setRenderFrameCallback(RenderFrameCallback renderFrameCallback) {
    this->renderFrameCallback = renderFrameCallback;
}