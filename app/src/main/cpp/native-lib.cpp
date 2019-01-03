#include <jni.h>
#include <string>
#include "DNFFmpeg.h"

DNFFmpeg *ffmpeg = 0;
extern "C"
JNIEXPORT void JNICALL
Java_com_chen_player_1ffmpeg_DNPlayer_native_1prepare(JNIEnv *env, jobject instance,
                                                      jstring dadataSource_) {
    const char *dadataSource = env->GetStringUTFChars(dadataSource_, 0);

    // TODO

    ffmpeg = new DNFFmpeg(dadataSource);

    env->ReleaseStringUTFChars(dadataSource_, dadataSource);
}