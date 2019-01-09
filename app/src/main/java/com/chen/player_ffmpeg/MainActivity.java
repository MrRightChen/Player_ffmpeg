package com.chen.player_ffmpeg;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {


    private DNPlayer dnPlayer;
    private SurfaceView mSurface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mSurface = (SurfaceView) findViewById(R.id.mSurface);


        dnPlayer = new DNPlayer();
        //dnPlayer.setDataSource("rtmp://live.hkstv.hk.lxdns.com/live/hks");
        dnPlayer.setDataSource("rtmp://58.200.131.2:1935/livetv/hunantv");

        dnPlayer.setSurfaceView(mSurface);

        dnPlayer.setOnPrepareListener(new DNPlayer.OnPrepareListener() {
            @Override
            public void onPrepare() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this,"准备好了",Toast.LENGTH_SHORT).show();
                    }
                });
                dnPlayer.start();
            }
        });

    }

    /**
     * 点击播放
     * @param view
     */
    public void start(View view) {

        dnPlayer.prepare();

    }

}
