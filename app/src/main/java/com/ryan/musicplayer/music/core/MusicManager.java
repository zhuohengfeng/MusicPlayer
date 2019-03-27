package com.ryan.musicplayer.music.core;

import com.ryan.musicplayer.callback.INativeCallback;
import com.ryan.musicplayer.utils.Logger;

public class MusicManager {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
//        System.loadLibrary("avcodec-57");
//        System.loadLibrary("avdevice-57");
//        System.loadLibrary("avfilter-6");
//        System.loadLibrary("avformat-57");
//        System.loadLibrary("avutil-55");
//        System.loadLibrary("postproc-54");
//        System.loadLibrary("swresample-2");
//        System.loadLibrary("swscale-4");
    }

    private INativeCallback mNativeCallback;


    public void onNativeCallback(int code, String message) {
        Logger.d("onNativeCallback code="+code+", message="+message);
        if (mNativeCallback != null) {
            mNativeCallback.onPrepared();
        }
    }


    public void setNativeCallback(INativeCallback callback) {
        mNativeCallback = callback;
    }


    /** 准备解码器 */
    public native void prepare(String url);
    /** 开始进行播放 */
    public native void start();


}
