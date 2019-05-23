//
// Created by hengfeng zhuo on 2019/3/27.
//

#ifndef MUSICPLAYER_DDCALLJAVA_H
#define MUSICPLAYER_DDCALLJAVA_H

#include <jni.h>

#define MAIN_THREAD 0
#define CHILD_THREAD 1

class DdCallJava {

public:
    JavaVM* javaVM = NULL;
    JNIEnv* jniEnv = NULL;
    jobject jobj;

    jmethodID jmid;

public:
    DdCallJava(JavaVM* jvm, JNIEnv* jEnv, jobject obj);
    virtual ~DdCallJava();

    // 回调给java，通知解码器准备好了
    void onCallPrepared(int type);

};

#endif //MUSICPLAYER_DDCALLJAVA_H
