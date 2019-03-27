//
// Created by hengfeng zhuo on 2019/3/25.
//

#ifndef MUSICPLAYER_DDFFMPEG_H
#define MUSICPLAYER_DDFFMPEG_H

#include <pthread.h>

extern "C"
{
#include <libavformat/avformat.h>
}

class DdFFmpeg {

public:
    const char* URL;
    pthread_t* decodeThread;
    AVFormatContext* pAvFormatContext;



public:
    DdFFmpeg(const char* url);
    virtual ~DdFFmpeg();

    void prepare(void);
    void start(void);
};

#endif //MUSICPLAYER_DDFFMPEG_H
