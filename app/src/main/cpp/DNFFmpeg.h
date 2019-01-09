//
// Created by Administrator on 2019/1/3.
//





#ifndef PLAYER_FFMPEG_DNFFMPEG_H
#define PLAYER_FFMPEG_DNFFMPEG_H

#include "JavaCallHelper.h"
#include "AudioChannel.h"
#include "VideoChannel.h"

extern "C"{
#include <libavformat/avformat.h>
};

#endif //PLAYER_FFMPEG_DNFFMPEG_H
class DNFFmpeg{

public:
    DNFFmpeg(JavaCallHelper* callHelper,const char* dataSource);
    ~DNFFmpeg();

    void prepare();
    void _prepare();
    void start();
    void _start();

    //设置渲染帧画面回调
    void setRenderFrameCallback(RenderFrameCallback callback);



private:
    char * dataSource;
    pthread_t  pid;
    pthread_t  pid_paly;
    AVFormatContext *formatContext =0;
    JavaCallHelper* callHelper;

    AudioChannel *audioChannel =0;
    VideoChannel *videoChannel =0;
    bool  isPlaying;
    RenderFrameCallback callback;



};