#include <jni.h>
#include <string>
#include <pthread.h>
#include "AndroidLog.h"
#include "DdFFmpeg.h"
#include "DdCallJava.h"
#include "DdPlayStatus.h"

extern "C"
{
#include <libavformat/avformat.h>
}

JavaVM* javaVM;

DdFFmpeg *pFFmpeg = NULL;
DdCallJava *pCallJava = NULL;
DdPlayStatus *pPlayStatus = NULL;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    javaVM = vm;
    JNIEnv *env;
    if(vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK)
    {
        return -1;
    }
    return JNI_VERSION_1_6;
}

/**
 * 开始解码，获取音频流---得到解码器---获取AVPackage---获取AVFrame---重采样的到PCM
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_ryan_musicplayer_music_core_MusicManager_prepare(JNIEnv *env, jobject instance,
                                                          jstring url_) {
    // 得到要播放的歌曲的URL
    const char* source_url = env->GetStringUTFChars(url_, NULL);

    // 创建一个NATIVE回调到JAVA的工具类DdCallJava
    if (pCallJava == NULL) {
        pCallJava = new DdCallJava(javaVM, env, instance);
    }

    // 创建一个播放状态类，可以设置退出标志
    if (pPlayStatus == NULL) {
        pPlayStatus = new DdPlayStatus();
    }

    // 实际解码的类
    if (pFFmpeg == NULL) {
        pFFmpeg = new DdFFmpeg(pCallJava, pPlayStatus, source_url);
    }

    pFFmpeg->prepare();
}


/**
 * 解码完成后，开始播放
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_ryan_musicplayer_music_core_MusicManager_start(JNIEnv *env, jobject instance) {

    if (pFFmpeg != NULL) {
        pFFmpeg->start();
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_com_ryan_musicplayer_music_core_MusicManager_pause(JNIEnv *env, jobject instance) {

    if (pFFmpeg != NULL) {
        pFFmpeg->pause();
    }

}extern "C"
JNIEXPORT void JNICALL
Java_com_ryan_musicplayer_music_core_MusicManager_resume(JNIEnv *env, jobject instance) {

    if (pFFmpeg != NULL) {
        pFFmpeg->resume();
    }

}extern "C"
JNIEXPORT void JNICALL
Java_com_ryan_musicplayer_music_core_MusicManager_stop(JNIEnv *env, jobject instance) {

    if (pFFmpeg != NULL) {
        pFFmpeg->stop();
    }

}