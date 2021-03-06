package com.chen.player_ffmpeg;

/**
 * Created by Administrator on 2019/1/3.
 */

import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * 提供了java 进行播放、停止等函数
 */
public class DNPlayer implements SurfaceHolder.Callback {


    static {
        System.loadLibrary("native-lib");
    }


    private String dadataSource;
    private SurfaceHolder holder;


    public void setDataSource(String dataSource){
        this.dadataSource = dataSource;
    }

    /**
     * 开始播放
     */
    public void start(){
        Log.e("DNPlayer:","start");
        nativeStart();

    }

    /**
     * 停止播放
     */
    public void stop(){

    }

    /**
     * 释放
     */
    public void release(){
        holder.removeCallback(this);

    }

    /**
     * 设置播放显示的画布
     * @param mSurface
     */

    public void setSurfaceView(SurfaceView mSurface) {

        holder = mSurface.getHolder();
        holder.addCallback(this);


    }

    /**
     * 准备好 要播放的视频
     */
    public void prepare() {

        nativePrepare(dadataSource);
    }

    /**
     * 创建画布
     * @param holder
     */
    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    /**
     * 画布改变了:按Home、横竖屏切换 都会回调该函数
     * @param holder
     * @param format
     * @param width
     * @param height
     */
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        nativeSetSurface(holder.getSurface());
    }

    /**
     * 画布销毁：按了Home、退出应用
     * @param holder
     */
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public void onError(int errorCode){
        //TODO 错误回调

    }

    public void onPrepare(){
        if (onPrepareListener!=null){
            onPrepareListener.onPrepare();
        }
    }
    //java与java之间回调接口
    private OnPrepareListener onPrepareListener;
    public void setOnPrepareListener(OnPrepareListener onPrepareListener) {
        this.onPrepareListener = onPrepareListener;
    }
    public interface OnPrepareListener{

        void onPrepare();

    }

    native void nativePrepare(String dadataSource);
    native void nativeStart();
    native void nativeSetSurface(Surface surface);



}
