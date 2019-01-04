//
// Created by Administrator on 2019/1/4.
//

#include "JavaCallHelper.h"
#include "macro.h"

JavaCallHelper::JavaCallHelper(JavaVM *vm, JNIEnv *env, jobject instance) {
    this->vm=vm;
    //如果主线程回调
    this->env=env;
    //一旦涉及到jobject类型的参数 跨方法、跨线程 就需要创建全局引用
    this->instance =env->NewGlobalRef(instance);


    jclass clazz = env->GetObjectClass(instance);
    onErrorID = env->GetMethodID(clazz, "onError", "(I)V");
    onPrepareID = env->GetMethodID(clazz,"onPrepare","()V");
}

JavaCallHelper::~JavaCallHelper() {

    env->DeleteGlobalRef(instance);
}

void JavaCallHelper::onError(int thread,int errorCode) {

    if (thread==THREAD_MAIN) {

        //主线程
        env->CallVoidMethod(instance,onErrorID,errorCode);
    }else{
        //子线程
        JNIEnv *env;
        //获得我当前线程的JNIEnv
        vm->AttachCurrentThread(&env,0);
        env->CallVoidMethod(instance,onErrorID,errorCode);
        vm->DetachCurrentThread();
    }

}

void JavaCallHelper::onPrepare(int thread) {
    if (thread==THREAD_MAIN) {
        //主线程
        env->CallVoidMethod(instance,onPrepareID);
    }else{

        //子线程
        JNIEnv *env;
        //获得我当前线程的JNIEnv
        vm->AttachCurrentThread(&env,0);
        env->CallVoidMethod(instance,onPrepareID);
        vm->DetachCurrentThread();


    }
}