//
// Created by Administrator on 2019/1/2.
//

#ifndef PLAYER_FFMPEG_SAFE_QUEUE_H
#define PLAYER_FFMPEG_SAFE_QUEUE_H

#endif //PLAYER_FFMPEG_SAFE_QUEUE_H

#include <queue>
#include <pthread.h>
using namespace std;

template <typename T>
class SafeQueue{
    typedef void(*ReleaseCallBack)(T&);
public:
    SafeQueue(){

        pthread_mutex_init(&mutex,0);
        pthread_cond_init(&cond,0);
    }
    ~SafeQueue(){
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }


    void push(T value){
        pthread_mutex_lock(&mutex);
        q.push(value);
        //条件变量通知 有了新数据
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

    }

    int pop(T& value){
        int ret = 0;
        pthread_mutex_lock(&mutex);
        while (q.empty()){
            pthread_cond_wait(&cond,&mutex);
        }
        if (!q.empty()){
            value = q.front();
            q.pop();
            ret = 1;
        }
        pthread_mutex_unlock(&mutex);
        return ret;
    }

    void clear(){
        pthread_mutex_lock(&mutex);
        uint32_t  size = q.size();
        for (int i = 0; i < size; ++i) {

            T value = q.front();
            if (releaseCallBack){
                //释放value
                releaseCallBack(value);
            }
            q.pop();


        }



    }

    void  setReleaseCallBack(ReleaseCallBack releaseCallBack){

        this->releaseCallBack = releaseCallBack;


    }



private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    queue<T> q;
    ReleaseCallBack releaseCallBack;

};