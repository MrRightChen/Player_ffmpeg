//
// Created by Administrator on 2019/1/8.
//

#ifndef PLAYER_FFMPEG_BASECHANNEL_H
#define PLAYER_FFMPEG_BASECHANNEL_H

extern "C"{
#include <libavcodec/avcodec.h>

};
#include "safe_queue.h"

class BaseChannel{
public:
    BaseChannel(int id):id(id){}

    //虚函数 子类需要调用虚构方法
    virtual ~BaseChannel(){
        packets.setReleaseCallback(BaseChannel::releasePacket);
        packets.clear();


    }
    static void releasePacket(AVPacket** packet){

        if (packet){
            av_packet_free(packet);
            *packet=0;
        }

    }

    int id;

    SafeQueue<AVPacket*> packets;



};

#endif //PLAYER_FFMPEG_BASECHANNEL_H
