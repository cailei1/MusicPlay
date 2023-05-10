package com.cailei.musicplay

/**
 *  author : cailei
 *  date : 2022/9/7
 *  description :
 */

interface CompressCallback {
    fun onCompress(current: Int, total: Int)
}


class VideoCompress {


    external fun compressVideo(compressCommand:Array<String>,compressCallback:CompressCallback)


}