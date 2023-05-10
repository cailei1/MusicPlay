package com.cailei.musicplay.music

import com.cailei.musicplay.listener.PlayerErrorListener

/**
 *  author : cailei
 *  date : 2023/5/9
 *  description :
 */
class MusicPlayer(private var url: String = "") {

    lateinit var onError: PlayerErrorListener

    init {
        System.loadLibrary("music-player");
    }

    fun setDataSource(url: String) {
        this.url = url
    }


    fun play() {
        nPlay(url)
    }

    private external fun nPlay(url: String)
    fun setOnPlayerErrorListener(onError: PlayerErrorListener) {
        this.onError = onError
    }


    fun onJniCallError(code:Int,msg:String){
        onError.onPlayerError(code,msg)
    }


}