//
// Created by hengfeng zhuo on 2019/3/27.
//
#include "AndroidLog.h"
#include "DdCallJava.h"

DdCallJava::DdCallJava(JavaVM *jvm, JNIEnv *jEnv, jobject obj) {
    this->javaVM = jvm;
    this->jniEnv = jEnv;
    this->jobj = jEnv->NewGlobalRef(obj);

    jclass jcls = jEnv->GetObjectClass(obj);
    if (jcls == NULL) {
        return;
    }
    // 获取java层回调方法
    this->jmid = jEnv->GetMethodID(jcls, "onNativeCallback", "(ILjava/lang/String;)V");
}

DdCallJava::~DdCallJava() {

}


void DdCallJava::onCallPrepared(int type) {
    // 在主线程回调
    if (type == MAIN_THREAD) {
        this->jniEnv->CallVoidMethod(this->jobj, this->jmid);
    }
    // 在子线程回调
    else if (type == CHILD_THREAD) {
        JNIEnv *jenv;
        if (this->javaVM->AttachCurrentThread(&jenv, 0) != JNI_OK) {
            LOGE("在子线程无法回调java");
            return;
        }
        jenv->CallVoidMethod(this->jobj, this->jmid);
        this->javaVM->DetachCurrentThread();
    }
}


