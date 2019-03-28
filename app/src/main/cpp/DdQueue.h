//
// Created by hengfeng zhuo on 2019/3/28.
//

#ifndef MUSICPLAYER_DDQUEUE_H
#define MUSICPLAYER_DDQUEUE_H

#include "DdPlayStatus.h"
#include "pthread.h"
#include "queue"

extern "C"
{
#include "libavcodec/avcodec.h"
};

class DdQueue {

public:
    DdPlayStatus* pPlayStatus = NULL;
    std::queue<AVPacket*> queuePacket;

    pthread_cond_t condPacket;
    pthread_mutex_t mutexPacket;

public:
    DdQueue(DdPlayStatus* status);
    virtual ~DdQueue();

    int putAvPacket(AVPacket* packet_in);
    int getAvPacket(AVPacket* packet_out);

    int getQueueSize();
};

#endif //MUSICPLAYER_DDQUEUE_H
