//
// Created by hengfeng zhuo on 2019/3/25.
//

#include "AndroidLog.h"
#include "DdFFmpeg.h"

DdFFmpeg::DdFFmpeg(const char* url) {
    this->URL = url;
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

    av_register_all();

    ffmpeg->pAvFormatContext = avformat_alloc_context();

    avformat_open_input(&ffmpeg->pAvFormatContext, ffmpeg->URL, NULL, NULL);

    if (avformat_find_stream_info(ffmpeg->pAvFormatContext, NULL) != 0) {
        LOGE("查找steam info失败");
        return NULL;
    }


    // 退出时要关闭，否则会crash
    pthread_exit(ffmpeg->decodeThread);
}


/**
 * 开始准备解码器
 */
void DdFFmpeg::prepare() {
    // 启动一个线程开始获取url的解码器
    pthread_create(this->decodeThread, NULL, initDecodeRunnable, this);
}


/**
 * 开始播放
 */
void DdFFmpeg::start(void) {

}