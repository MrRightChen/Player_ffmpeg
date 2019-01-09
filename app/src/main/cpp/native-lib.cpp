#include <jni.h>
#include <string>
#include "DNFFmpeg.h"
#include "JavaCallHelper.h"
#include <android/native_window_jni.h> //本地窗口 头文件
#include "macro.h"

DNFFmpeg *ffmpeg = 0;
//拿到JavaVM 对象
JavaVM *javaVM=0;
ANativeWindow *window = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int JNI_OnLoad(JavaVM *vm,void *r){
    javaVM = vm;
    return JNI_VERSION_1_6;
}

//画画 回调出来
void render(uint8_t *data, int lineszie, int w, int h) {
    pthread_mutex_lock(&mutex);
    if (!window) {
        pthread_mutex_unlock(&mutex);
        return;
    }
    //设置窗口属性
    ANativeWindow_setBuffersGeometry(
            window,
            w,
            h,
            WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer window_buffer;
    if (ANativeWindow_lock(window, &window_buffer, 0)) {
        ANativeWindow_release(window);
        window = 0;
        pthread_mutex_unlock(&mutex);
        return;
    }
    //填充rgb数据给dst_data
    uint8_t *dst_data = static_cast<uint8_t *>(window_buffer.bits);
    // stride：一行多少个数据（RGBA） *4
    int dst_linesize = window_buffer.stride * 4;
    //一行一行的拷贝
    for (int i = 0; i < window_buffer.height; ++i) {
        //memcpy(dst_data , data, dst_linesize);
        memcpy(dst_data + i * dst_linesize, data + i * lineszie, dst_linesize);
    }
    ANativeWindow_unlockAndPost(window);
    pthread_mutex_unlock(&mutex);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_chen_player_1ffmpeg_DNPlayer_nativePrepare(JNIEnv *env, jobject instance,
                                                      jstring dadataSource_) {
    const char *dadataSource = env->GetStringUTFChars(dadataSource_, 0);

    JavaCallHelper *callHelper = new JavaCallHelper(javaVM, env, instance);
    ffmpeg = new DNFFmpeg(callHelper,dadataSource);
    ffmpeg->setRenderFrameCallback(render);
    ffmpeg->prepare();
    env->ReleaseStringUTFChars(dadataSource_, dadataSource);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_chen_player_1ffmpeg_DNPlayer_nativeStart(JNIEnv *env, jobject instance) {

    LOGE("nativeStart");
    ffmpeg->start();

}


extern "C"
JNIEXPORT void JNICALL
Java_com_chen_player_1ffmpeg_DNPlayer_nativeSetSurface(JNIEnv *env, jobject instance,
                                                       jobject surface) {
    pthread_mutex_lock(&mutex);
    if (window){
        //释放老的window 屏幕切换 导致的
        ANativeWindow_release(window);
        window =0;
    }
    window =ANativeWindow_fromSurface(env,surface);
    pthread_mutex_unlock(&mutex);
}