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
    DdAudio* pAudio;
    DdCallJava* pCallJava;
    DdPlayStatus* pPlayStatus;

    const char* URL;
    pthread_t decodeThread;
    AVFormatContext* pAvFormatContext;


public:
    DdFFmpeg(DdCallJava *pCallJava, DdPlayStatus *pPlayStatus, const char *url);
    virtual ~DdFFmpeg();

    void prepare(void);
    void start(void);

    // 具体的解码操作
    void decodeFFmpegThread();

};

#endif //MUSICPLAYER_DDFFMPEG_H
