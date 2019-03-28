//
// Created by hengfeng zhuo on 2019/3/27.
//

#ifndef MUSICPLAYER_DDAUDIO_H
#define MUSICPLAYER_DDAUDIO_H

#include "DdPlayStatus.h"
#include "DdQueue.h"

extern "C" {
#include "libavcodec/avcodec.h"
};

class DdAudio {

public:
    int streamIndex = -1; // 保存音频流id
    AVCodecContext* pAVCodecContext = NULL;
    AVCodecParameters* pCodecParameters = NULL;
    DdQueue* pQueue = NULL;
    DdPlayStatus* pPlayStatus = NULL;

public:
    DdAudio(DdPlayStatus* playStatus);
    virtual ~DdAudio();
};

#endif //MUSICPLAYER_DDAUDIO_H
