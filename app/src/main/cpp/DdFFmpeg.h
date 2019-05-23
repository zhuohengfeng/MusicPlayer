//
// Created by hengfeng zhuo on 2019/3/25.
//

#ifndef MUSICPLAYER_DDFFMPEG_H
#define MUSICPLAYER_DDFFMPEG_H

#include <pthread.h>
#include "DdCallJava.h"
#include "DdPlayStatus.h"
#include "DdAudio.h"

extern "C"
{
#include "libavformat/avformat.h"
//#include "libavcodec/avcodec.h"
}


class DdFFmpeg {

public:
    DdAudio* pAudio = NULL;
    DdCallJava* pCallJava = NULL;
    DdPlayStatus* pPlayStatus = NULL;

    const char* URL;

    // 注意，这里不是指针类型，所以在创建这个实例的时候，decodeThread就会创建完成！！和java不一样！！！
    pthread_t decodeThread;
    // 注意，这里指针类型，所以创建实例的时候，这个指针没有指向具体的实例
    AVFormatContext* pAvFormatContext = NULL;


public:
    DdFFmpeg(DdCallJava *pCallJava, DdPlayStatus *pPlayStatus, const char *url);
    virtual ~DdFFmpeg();

    void prepare(void);
    void start(void);

    // 具体的解码操作
    void decodeFFmpegThread();

};

#endif //MUSICPLAYER_DDFFMPEG_H
