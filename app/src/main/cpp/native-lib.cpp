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


extern "C"
JNIEXPORT void JNICALL
Java_com_ryan_musicplayer_music_core_MusicManager_prepare(JNIEnv *env, jobject instance,
                                                          jstring url_) {
    const char* source_url = env->GetStringUTFChars(url_, NULL);

    if (pCallJava == NULL) {
        pCallJava = new DdCallJava(javaVM, env, instance);
    }

    if (pPlayStatus == NULL) {
        pPlayStatus = new DdPlayStatus();
    }

    if (pFFmpeg == NULL) {
        pFFmpeg = new DdFFmpeg(pCallJava, pPlayStatus, source_url);
    }

    pFFmpeg->prepare();
}



extern "C"
JNIEXPORT void JNICALL
Java_com_ryan_musicplayer_music_core_MusicManager_start(JNIEnv *env, jobject instance) {

    if (pFFmpeg != NULL) {
        pFFmpeg->start();
    }

}

