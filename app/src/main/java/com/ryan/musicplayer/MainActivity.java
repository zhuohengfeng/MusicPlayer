package com.ryan.musicplayer;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import com.ryan.musicplayer.callback.INativeCallback;
import com.ryan.musicplayer.music.core.MusicManager;
import com.ryan.musicplayer.utils.Logger;

public class MainActivity extends AppCompatActivity implements INativeCallback {

    private MusicManager manager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        manager = new MusicManager();
        manager.setNativeCallback(this);
    }

    public void onPlayClick(View view) {
        manager.prepare("http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3");
    }

    @Override
    public void onPrepared() {
        Logger.d("解码器准备好了，开始播放");
        manager.start();
    }
}
