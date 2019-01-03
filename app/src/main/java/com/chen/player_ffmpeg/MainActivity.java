package com.chen.player_ffmpeg;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {


    private DNPlayer dnPlayer;
    private SurfaceView mSurface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mSurface = (SurfaceView) findViewById(R.id.mSurface);


        dnPlayer = new DNPlayer();
        dnPlayer.setDataSource("rtmp://live.hkstv.hk.lxdns.com/live/hks");
        dnPlayer.setSurfaceView(mSurface);


    }

    /**
     * 点击播放
     * @param view
     */
    public void start(View view) {

        dnPlayer.prepare();

    }

}
