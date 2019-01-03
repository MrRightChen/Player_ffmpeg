package com.chen.player_ffmpeg;

/**
 * Created by Administrator on 2019/1/3.
 */

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

        native_prepare(dadataSource);
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

    }

    /**
     * 画布销毁：按了Home、退出应用
     * @param holder
     */
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }



    native void native_prepare(String dadataSource);

}
