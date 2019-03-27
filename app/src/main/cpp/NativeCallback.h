//
// Created by hengfeng zhuo on 2019/3/25.
//

#ifndef MUSICPLAYER_NATIVECALLBACK_H
#define MUSICPLAYER_NATIVECALLBACK_H

#include "jni.h"
#include <string>

using namespace std;

class NativeCallback {

public:
    JNIEnv* env = NULL;
    JavaVM* vm = NULL;

    jobject obj;
    jclass  cls;

public:
    NativeCallback();
    virtual ~NativeCallback();

    string getName() { return "hello from jni, haha";};
};

#endif //MUSICPLAYER_NATIVECALLBACK_H
