//
// Created by Administrator on 2019/1/8.
//

#ifndef PLAYER_FFMPEG_BASECHANNEL_H
#define PLAYER_FFMPEG_BASECHANNEL_H
#include "safe_queue.h"
extern "C"{
#include <libavcodec/avcodec.h>
};


class BaseChannel{

public:
    BaseChannel(int id,
                AVCodecContext *avCodecContext)
            :id(id), avCodecContext(avCodecContext){
    }

    //虚函数 子类需要调用虚构方法
    virtual ~BaseChannel(){
        //作用域
        packets.setReleaseCallback(BaseChannel::releasePacket);
        packets.clear();
    }
    static void releasePacket(AVPacket** packet){
        if (packet){
            av_packet_free(packet);
            //为什么用指针的指针？
            // 指针的指针能够修改传递进来的指针的指向
            *packet=0;
        }

    }
    static void releaseFrame(AVFrame** frame){

        if (frame){
            av_frame_free(frame);
            //为什么用指针的指针？
            // 指针的指针能够修改传递进来的指针的指向
            *frame=0;
        }

    }

    //纯虚函数 相当于抽象方法 子类必须要实现
    virtual void play() = 0;

    int id;
    SafeQueue<AVPacket *> packets;
    //SafeQueue<AVFrame*> frames;
    bool isPlaying;
    AVCodecContext *avCodecContext;



};

#endif //PLAYER_FFMPEG_BASECHANNEL_H
