#include <jni.h>
#include <string>
#include "DNFFmpeg.h"
#include "JavaCallHelper.h"

DNFFmpeg *ffmpeg = 0;
//拿到JavaVM 对象
JavaVM *javaVM=0;
int JNI_OnLoad(JavaVM *vm,void *r){
    javaVM = vm;
    return JNI_VERSION_1_6;
}



extern "C"
JNIEXPORT void JNICALL
Java_com_chen_player_1ffmpeg_DNPlayer_nativePrepare(JNIEnv *env, jobject instance,
                                                      jstring dadataSource_) {
    const char *dadataSource = env->GetStringUTFChars(dadataSource_, 0);

    JavaCallHelper *callHelper = new JavaCallHelper(javaVM, env, instance);
    ffmpeg = new DNFFmpeg(callHelper,dadataSource);
    ffmpeg->prepare();
    env->ReleaseStringUTFChars(dadataSource_, dadataSource);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_chen_player_1ffmpeg_DNPlayer_nativeStart(JNIEnv *env, jobject instance) {

    ffmpeg->start();

}