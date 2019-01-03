//
// Created by Administrator on 2019/1/3.
//

#include <cstring>
#include "DNFFmpeg.h"
#include <pthread.h>

void* task_prepare(void *args){

    DNFFmpeg *dnfFmpeg = static_cast<DNFFmpeg *>(args);
    //这样话方便使用成员变量
    dnfFmpeg->_prepare();
    //必须加 不然出问题
    return 0;
}

DNFFmpeg::DNFFmpeg(const char *dataSource) {
    //这样写避免悬空指针 dataSource指向的内存被释放
    this->dataSource = new char[strlen(dataSource)];
    strcpy(this->dataSource,dataSource);
}

DNFFmpeg::~DNFFmpeg() {

    //释放
    delete dataSource;
    dataSource = 0;

}


void DNFFmpeg::prepare() {
    pthread_create(&pid,0,task_prepare,this);

}

void DNFFmpeg::_prepare() {
    //1、打开媒体地址（文件地址、直播地址）

    //AVFormatContext 包含视频的相关信息
    formatContext = 0;
    avformat_open_input(&formatContext,dataSource,0,0);


}