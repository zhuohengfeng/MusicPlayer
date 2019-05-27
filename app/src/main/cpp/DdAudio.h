//
// Created by hengfeng zhuo on 2019/3/27.
//

#ifndef MUSICPLAYER_DDAUDIO_H
#define MUSICPLAYER_DDAUDIO_H

#include "DdPlayStatus.h"
#include "DdQueue.h"

extern "C" {
#include "libavcodec/avcodec.h"

#include <libswresample/swresample.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
};

class DdAudio {

public:
    int streamIndex = -1; // 保存音频流id
    AVCodecContext* pAVCodecContext = NULL;
    AVCodecParameters* pCodecParameters = NULL;
    DdQueue* pQueue = NULL;
    DdPlayStatus* pPlayStatus = NULL;

    // 播放的子线程
    pthread_t thread_play;
    AVPacket* avPacket = NULL;
    AVFrame* avFrame = NULL;

    int ret = 0;
    uint8_t* buffer = NULL;
    int data_size = 0;
    int sample_rate = 0;

    // OpenSL ES 引擎接口
    SLObjectItf engineObject = NULL;
    SLEngineItf engineEngine = NULL;

    // OpenSL ES 混音器
    SLObjectItf outputMixObject = NULL;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;
    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    // pcm
    SLObjectItf pcmPlayerObject = NULL;
    SLPlayItf pcmPlayerPlay = NULL;

    //缓冲器队列接口
    SLAndroidSimpleBufferQueueItf pcmBufferQueue = NULL;


public:
    DdAudio(DdPlayStatus* playStatus, int sample_rate);
    virtual ~DdAudio();

    // 从队列中读取AVPackage，然后开始重采样成PCM数据，开始播放
    void play();

    int resampleAudio();

    void initOpenSLES();

    int getCurrentSampleRateForOpensles(int sample_rate);

    void pausePlay();
    void resumePlay();
    void stopPlay();
};

#endif //MUSICPLAYER_DDAUDIO_H
