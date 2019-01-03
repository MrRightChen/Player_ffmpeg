//
// Created by Administrator on 2019/1/3.
//



#ifndef PLAYER_FFMPEG_DNFFMPEG_H
#define PLAYER_FFMPEG_DNFFMPEG_H


extern "C"{
#include <libavformat/avformat.h>
};

#endif //PLAYER_FFMPEG_DNFFMPEG_H
class DNFFmpeg{

public:
    DNFFmpeg(const char* dataSource);
    ~DNFFmpeg();

    void prepare();
    void _prepare();



private:
    char * dataSource;
    pthread_t  pid;
    AVFormatContext *formatContext;


};