//
// Created by Administrator on 2019/1/4.
//

#ifndef PLAYER_FFMPEG_JAVACALLHELPER_H
#define PLAYER_FFMPEG_JAVACALLHELPER_H

#include <jni.h>

class JavaCallHelper {

public:
    JavaCallHelper(JavaVM *vm,JNIEnv* env,jobject instance);
    ~JavaCallHelper();
    //完成回调java
    void onError(int thread,int errorCode);
    void onPrepare(int thread);

private:
    JavaVM *vm;//跨线程使用
    JNIEnv *env;
    jobject instance;
    jmethodID onErrorID;
    jmethodID onPrepareID;


};


#endif //PLAYER_FFMPEG_JAVACALLHELPER_H
