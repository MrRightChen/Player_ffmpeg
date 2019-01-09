//
// Created by Administrator on 2019/1/3.
//

#include <cstring>
#include "DNFFmpeg.h"
#include "macro.h"
#include <pthread.h>

void* task_prepare(void *args){

    DNFFmpeg *dnfFmpeg = static_cast<DNFFmpeg *>(args);
    //这样话方便使用成员变量
    dnfFmpeg->_prepare();
    //必须加 不然出问题
    return 0;
}

DNFFmpeg::DNFFmpeg(JavaCallHelper* callHelper,const char *dataSource) {
    this->callHelper=callHelper;
    //这样写避免悬空指针 dataSource指向的内存被释放 注意：\0结尾
    this->dataSource = new char[strlen(dataSource)+1];
    strcpy(this->dataSource,dataSource);
}

DNFFmpeg::~DNFFmpeg() {

    //释放
    DELETE(dataSource)
    DELETE(callHelper)

}


void DNFFmpeg::prepare() {
    pthread_create(&pid,0,task_prepare,this);

}

void DNFFmpeg::_prepare() {
    //网络初始化 ffmpeg 使用网络
    avformat_network_init();

    //1、打开媒体地址（文件地址、直播地址）
    //AVFormatContext 包含视频的相关信息
    formatContext = 0;
    //文件路径不对或者手机没网 初始化失败了
    int ret = avformat_open_input(&formatContext,dataSource,0,0);

    //ret=0 打开媒体成功
    if (ret!=0){
        callHelper->onError(THREAD_CHILD,FFMPEG_CAN_NOT_OPEN_URL);
        LOGE("打开媒体失败:%s",av_err2str(ret));
        return;
    }
    //2、查找媒体中的音视频流 >=0成功
    ret = avformat_find_stream_info(formatContext,0);
    if (ret<0){
        callHelper->onError(THREAD_CHILD,FFMPEG_CAN_NOT_FIND_STREAMS);
        LOGE("查找媒体中的音视频流失败:%s",av_err2str(ret));
        return;
    }
    for (int i = 0; i <formatContext->nb_streams ; ++i) {
        //可能视频 可能是音频
        AVStream *stream =formatContext->streams[i];
        //包含了解码这段流各种参数信息
        AVCodecParameters *codecpar = stream->codecpar;
        //无论视频和音频都需要做的事情 ：通用的处理
        //1、通过当前流 使用 编码方式 找解码器
        AVCodec *dec = avcodec_find_decoder(codecpar->codec_id);
        if (dec==NULL){
            callHelper->onError(THREAD_CHILD,FFMPEG_FIND_DECODER_FAIL);
            LOGE("查找解码器失败");
            return;
        }
        //2、获得解码器的上下文
        AVCodecContext *context = avcodec_alloc_context3(dec);
        if (context==NULL){
            callHelper->onError(THREAD_CHILD,FFMPEG_ALLOC_CODEC_CONTEXT_FAIL);
            LOGE("获取解码器上下文失败");
            return;
        }
        //3、设置上下文的一些参数
        ret = avcodec_parameters_to_context(context, codecpar);
        if (ret<0){
            callHelper->onError(THREAD_CHILD,FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL);
            LOGE("设置上下文的一些参数失败:%s",av_err2str(ret));

            return;
        }
        //4、打开编码器
        ret = avcodec_open2(context,dec,0);
        if (ret<0){
            callHelper->onError(THREAD_CHILD,FFMPEG_OPEN_DECODER_FAIL);
            LOGE("打开解码器失败:%s",av_err2str(ret));
            return;
        }
        if (codecpar->codec_type==AVMEDIA_TYPE_AUDIO){
            //音频
            audioChannel  = new AudioChannel(i,context);
        }else if (codecpar->codec_type==AVMEDIA_TYPE_VIDEO){
            //视频
            videoChannel = new VideoChannel(i,context);
            videoChannel->setRenderFrameCallback(callback);
        }
    }
    //没有音视频 （很少见）
    if (!audioChannel&&!videoChannel){
        callHelper->onError(THREAD_CHILD,FFMPEG_NOMEDIA);
        LOGE("没有音视频");
        return;
    }
    //准备完了 通知java 准备好了，可以开始播放了
    callHelper->onPrepare(THREAD_CHILD);
    LOGE("有音视频");


}


void *play(void* args){

    DNFFmpeg *dnfFmpeg = static_cast<DNFFmpeg *>(args);

    dnfFmpeg->_start();

    return 0;

}

void DNFFmpeg::start() {
    //标记正在播放
    isPlaying =1;

    if(videoChannel){
        videoChannel->packets.setWork(1);
        videoChannel->play();
        pthread_create(&pid_paly,0,play,this);
    } else{
        LOGE("VideoChannel:");
    }
}
/**
 * 读取数据包
 */
void DNFFmpeg::_start() {

    //AVPacket:存放
    //AVFrame:

    int ret;
    //1、读取媒体数据包
    while(isPlaying){
        AVPacket *avPacket = av_packet_alloc();
        ret = av_read_frame(formatContext,avPacket);
        if (ret==0){
            //读取成功
            //stream_index :流的下标
            if (audioChannel&&avPacket ->stream_index == audioChannel->id){
                audioChannel->packets.push(avPacket);
            } else if (videoChannel&& avPacket->stream_index ==videoChannel->id){
                videoChannel->packets.push(avPacket);
            }
        }else if (ret == AVERROR_EOF){
            //读取完成，可能没播放完

        } else{
            //

        }
    }

}


void DNFFmpeg::setRenderFrameCallback(RenderFrameCallback callback) {
    this->callback = callback;
}
