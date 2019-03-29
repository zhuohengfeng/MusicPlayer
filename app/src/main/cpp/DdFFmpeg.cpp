//
// Created by hengfeng zhuo on 2019/3/25.
//

#include "AndroidLog.h"
#include "DdFFmpeg.h"

DdFFmpeg::DdFFmpeg(DdCallJava *pCallJava, DdPlayStatus *pPlayStatus, const char *url) {
    this->URL = url;
    this->pCallJava = pCallJava;
    this->pPlayStatus = pPlayStatus;
}

DdFFmpeg::~DdFFmpeg() {

    if (this->pAvFormatContext != NULL) {
        av_free(this->pAvFormatContext);
        this->pAvFormatContext = NULL;
    }
}

/**
 * 开始获取解码器的子线程
 */
void* initDecodeRunnable(void* data) {
    DdFFmpeg* ffmpeg = (DdFFmpeg*)data;

    ffmpeg->decodeFFmpegThread();

    // 退出时要关闭，否则会crash
    pthread_exit(&ffmpeg->decodeThread);
}


/**
 * 开始准备解码器
 */
void DdFFmpeg::prepare() {
    // 启动一个线程开始获取url的解码器
    LOGD("Native prepare()");
    pthread_create(&this->decodeThread, NULL, initDecodeRunnable, this);
}



void DdFFmpeg::decodeFFmpegThread() {
    LOGD("开始执行解码流程...decodeFFmpegThread");

    // 注册
    av_register_all();

    // 初始化网络
    avformat_network_init();

    this->pAvFormatContext = avformat_alloc_context();

    // 打开输入的流
    if (avformat_open_input(&this->pAvFormatContext, this->URL, NULL, NULL) != 0) {
        LOGE("无法打开URL流 %s", this->URL);
        return;
    }

    // 找到音频流
    for (int i = 0; i < this->pAvFormatContext->nb_streams; i++) {
        if (pAvFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (pAudio == NULL) {
                pAudio = new DdAudio(this->pPlayStatus);
                pAudio->streamIndex = i;
                pAudio->pCodecParameters = pAvFormatContext->streams[i]->codecpar;
            }
        }
    }

    // 开始查找解码器
    AVCodec* decodec = avcodec_find_decoder(pAudio->pCodecParameters->codec_id);
    if (!decodec) {
        LOGE("找不到解码器");
        return;
    }

    // 分配AV codec
    pAudio->pAVCodecContext = avcodec_alloc_context3(decodec);
    if (!pAudio->pAVCodecContext) {
        LOGE("AVCodecContext 分配失败");
        return;
    }

    if (avcodec_parameters_to_context(pAudio->pAVCodecContext, pAudio->pCodecParameters) < 0) {
        LOGE("填充decodec 到context失败");
        return;
    }

    if (avcodec_open2(pAudio->pAVCodecContext, decodec, NULL) != 0) {
        LOGE("无法打开audio 流");
        return;
    }

    LOGD("解码流程结束...通知java层");
    this->pCallJava->onCallPrepared(CHILD_THREAD);
}



/**
 * 开始播放
 */
void DdFFmpeg::start(void) {
    LOGD("开始播放....");

    if (this->pAudio != NULL) {
        LOGE("pAudio为空， 退出");
        return;
    }

    int count = 0;
    while(1) {
        AVPacket* avPacket = av_packet_alloc();
        if (av_read_frame(this->pAvFormatContext, avPacket) == 0) {
            if (avPacket->stream_index == pAudio->streamIndex) {
                count++;
                LOGD("解码第%d帧", count);
                pAudio->pQueue->putAvPacket(avPacket);
            }
            else {
                av_packet_free(&avPacket);
                av_free(avPacket);
            }
        }
        else {
            LOGD("解码完成");
            av_packet_free(&avPacket);
            av_free(avPacket);
            break;
        }
    }


    //模拟出队
    while (pAudio->pQueue->getQueueSize() > 0)
    {
        AVPacket *packet = av_packet_alloc();
        pAudio->pQueue->getAvPacket(packet);
        av_packet_free(&packet);
        av_free(packet);
        packet = NULL;
    }
    LOGD("解码完成");


}