//
// Created by hengfeng zhuo on 2019/3/25.
//

#include "AndroidLog.h"
#include "DdFFmpeg.h"

/**
 * 实际解码类
 * @param pCallJava  回调java接口
 * @param pPlayStatus 当前解码和播放状态标志
 * @param url  要解码的URL
 */
DdFFmpeg::DdFFmpeg(DdCallJava *pCallJava, DdPlayStatus *pPlayStatus, const char *url) {
    this->URL = url;
    this->pCallJava = pCallJava;
    this->pPlayStatus = pPlayStatus;
}

DdFFmpeg::~DdFFmpeg() {
    this->pPlayStatus->exit = true;
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
 * 开始准备解码器，启动一个子线程
 */
void DdFFmpeg::prepare() {
    // 启动一个线程开始获取url的解码器，把自己(DDFfmpeg作为参数传递给子线程)
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
    LOGD("打开输入的流...%s", this->URL);
    if (avformat_open_input(&this->pAvFormatContext, this->URL, NULL, NULL) != 0) {
        LOGE("无法打开URL流 %s", this->URL);
        return;
    }

    // 查找流
    if(avformat_find_stream_info(this->pAvFormatContext, NULL) < 0) {
        LOGE("找不到流信息 %s", this->URL);
        return;
    }

    // 找到音频流
    LOGD("查找音频流...nb_streams = %d", this->pAvFormatContext->nb_streams);
    for (int i = 0; i < this->pAvFormatContext->nb_streams; i++) {
        if (pAvFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) { //得到音频流
            if (pAudio == NULL) {
                pAudio = new DdAudio(this->pPlayStatus, pAvFormatContext->streams[i]->codecpar->sample_rate);
                pAudio->streamIndex = i;
                pAudio->pCodecParameters = pAvFormatContext->streams[i]->codecpar;
            }
        }
    }

    // 开始查找解码器
    LOGD("查找解码器...%d", pAudio->pCodecParameters->codec_id);
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

    LOGD("打开audio 流...");
    if (avcodec_open2(pAudio->pAVCodecContext, decodec, NULL) != 0) {
        LOGE("无法打开audio 流");
        return;
    }

    LOGD("解码流程结束..找到解码器..通知java层");
    this->pCallJava->onCallPrepared(CHILD_THREAD);
}



/**
 * 开始播放
 */
void DdFFmpeg::start(void) {
    LOGD("开始播放....");

    if (this->pAudio == NULL) {
        LOGE("pAudio为空， 退出");
        return;
    }

    // 可以先开始播放，因为是2个线程，一个线程解码并把PCM放入buffer，另外一个线程播放buffer中的PCM
    this->pAudio->play();

    int count = 0;
    while(this->pPlayStatus != NULL && !this->pPlayStatus->exit) {
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
            LOGE("解码第%d帧失败，完成解码, 全部放入队列中了", count);
            av_packet_free(&avPacket);
            av_free(avPacket);
            // 读取失败，等待队列读取完成
            while(this->pPlayStatus != NULL && !this->pPlayStatus->exit) {
                if(pAudio->pQueue->getQueueSize() > 0) {
                    continue;
                } else {
                    this->pPlayStatus->exit = true;
                    break;
                }
            }
        }
    }

    LOGD("解码完成");
}