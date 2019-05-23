package com.ryan.musicplayer;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import com.blankj.utilcode.util.LogUtils;
import com.blankj.utilcode.util.Utils;
import com.ryan.musicplayer.callback.INativeCallback;
import com.ryan.musicplayer.music.core.MusicManager;

public class MainActivity extends AppCompatActivity implements INativeCallback {

    private MusicManager manager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        manager = new MusicManager();
        manager.setNativeCallback(this);

        Utils.init(this.getApplication());
    }

    public void onPlayClick(View view) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                manager.prepare("http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3");
            }
        }).start();
    }

    /**
     * 解码器已经准备好了，可以开始播放
     * 读取队列中的AVPackage----转成AVFrame----在转成PCM----用opengsl es播放
     */
    @Override
    public void onPrepared() {
        LogUtils.d("zhf_music JAVA: 解码器准备好了，开始播放");
        new Thread(new Runnable() {
            @Override
            public void run() {
                manager.start();
            }
        }).start();
    }
}
