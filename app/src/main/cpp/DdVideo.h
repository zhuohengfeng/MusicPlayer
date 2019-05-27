//
// Created by hengfeng zhuo on 2019-05-27.
//

#ifndef MUSICPLAYER_DDVIDEO_H
#define MUSICPLAYER_DDVIDEO_H

#include "DdPlayStatus.h"
#include "DdQueue.h"

extern "C" {
#include "libavcodec/avcodec.h"

};

class DdVideo {

public:
    AVCodecContext* pAvCodeContext = NULL;
    DdPlayStatus* pPlayStatus = NULL;
    DdQueue* pQueue = NULL;

public:
    DdVideo();
    virtual ~DdVideo();



};


#endif //MUSICPLAYER_DDVIDEO_H
