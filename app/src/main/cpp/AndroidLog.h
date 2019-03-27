//
// Created by hengfeng zhuo on 2019/3/25.
//

#ifndef MUSICPLAYER_DDAUDIO_H
#define MUSICPLAYER_DDAUDIO_H

#include "android/log.h"

#define LOG_DEBUG true

#define LOGD(FORMAT,...) __android_log_print(ANDROID_LOG_DEBUG,"zhf_music",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"zhf_music",FORMAT,##__VA_ARGS__);


#endif //MUSICPLAYER_DDAUDIO_H