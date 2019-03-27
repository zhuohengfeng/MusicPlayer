#include <jni.h>
#include <string>
#include <pthread.h>
#include "AndroidLog.h"
#include "DdFFmpeg.h"


#include "NativeCallback.h"

extern "C"
{
//注意这里用<>导入
#include <libavformat/avformat.h>
}

pthread_t gpthread;

NativeCallback gNativeCallback;

JavaVM* jVM;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    jVM = vm;
    JNIEnv *env;
    if(vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK)
    {
        return -1;
    }
    return JNI_VERSION_1_6;
}

void* callBackToJavaFun(void* data) {
    LOGD("启动了子线程callBackToJavaFun");
    JNIEnv* env;
    jVM->AttachCurrentThread(&env, 0);

    jobject jobj = (jobject)(data);
    jclass clz = env->GetObjectClass(jobj);
    jmethodID jmid = env->GetMethodID(clz, "onNativeCallback", "(ILjava/lang/String;)V");
    jstring jmsg = env->NewStringUTF("In child thread");
    env->CallVoidMethod(jobj, jmid, 200, jmsg);
    env->DeleteLocalRef(jmsg);
    jVM->DetachCurrentThread();
    pthread_exit(&gpthread);
}



extern "C" JNIEXPORT jstring JNICALL
Java_com_ryan_musicplayer_music_core_MusicManager_stringFromJNI(JNIEnv *env, jobject jobj/* this */) {

    // TODO
//    av_register_all();
//    // 打印所有的编解码器
//    AVCodec *c_temp = av_codec_next(NULL);
//    while (c_temp != NULL)
//    {
//        switch (c_temp->type)
//        {
//            case AVMEDIA_TYPE_VIDEO:
//                //LOGI("[Video]:%s", c_temp->name);
//                break;
//            case AVMEDIA_TYPE_AUDIO:
//                //LOGI("[Audio]:%s", c_temp->name);
//                break;
//            default:
//                //LOGI("[Other]:%s", c_temp->name);
//                break;
//        }
//        c_temp = c_temp->next;
//    }

    std::string hello = gNativeCallback.getName();
    LOGD("Java_com_ryan_musicplayer_music_core_MusicManager_stringFromJNI");

    // 首先在主线程回调Demo.java中的方法
    jobject globalObj = env->NewGlobalRef(jobj);
    jclass clz = env->GetObjectClass(globalObj);
    if(!clz)
    {
        LOGD("clz 为空");
        return env->NewStringUTF(hello.c_str());
    }
    jmethodID jmid = env->GetMethodID(clz, "onNativeCallback", "(ILjava/lang/String;)V");
    if(!jmid)
    {
        LOGD("jmid为空");
        return env->NewStringUTF(hello.c_str());
    }

    jstring jmsg = env->NewStringUTF("test");
    env->CallVoidMethod(globalObj, jmid, 100, jmsg);//
    env->DeleteLocalRef(jmsg);

    // 启动一个线程来回调Demo.java中的方法
    pthread_create(&gpthread, NULL, callBackToJavaFun, globalObj);

    return env->NewStringUTF(hello.c_str());
}





JavaVM *javaVM = NULL;
//WlCallJava *callJava = NULL;
DdFFmpeg *pFFmpeg = NULL;

extern "C"
JNIEXPORT void JNICALL
Java_com_ryan_musicplayer_music_core_MusicManager_prepare(JNIEnv *env, jobject instance,
                                                          jstring url_) {
    const char* source_url = env->GetStringUTFChars(url_, NULL);

    if (pFFmpeg == NULL) {
        pFFmpeg = new DdFFmpeg(source_url);
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

