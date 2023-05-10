package com.cailei.musicplay.listener

/**
 *  author : cailei
 *  date : 2023/5/10
 *  description :
 */
interface PlayerErrorListener {
    fun onPlayerError(code: Int, msg: String)
}