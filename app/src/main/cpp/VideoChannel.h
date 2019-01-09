//
// Created by Administrator on 2019/1/4.
//

#ifndef PLAYER_FFMPEG_VIDEOCHANNEL_H
#define PLAYER_FFMPEG_VIDEOCHANNEL_H


#include "BaseChannel.h"
extern "C"{
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
};
typedef void (*RenderFrameCallback)(uint8_t *,int,int,int);
class VideoChannel: public BaseChannel {

public:

    VideoChannel(int id, AVCodecContext *avCodecContext);
    ~VideoChannel();

    //解码+ 播放
    void play();
    void decode();
    void render();
    void setRenderFrameCallback(RenderFrameCallback renderFrameCallback);


private:
    pthread_t pid_decode;
    pthread_t pid_render;
    SwsContext *swsContext = 0;
    SafeQueue<AVFrame*> frames;
    RenderFrameCallback renderFrameCallback;
};
#endif //PLAYER_FFMPEG_VIDEOCHANNEL_H
