package com.cailei.musicplay

import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.cailei.musicplay.databinding.ActivityMainBinding
import com.cailei.musicplay.listener.PlayerErrorListener
import com.cailei.musicplay.music.MusicPlayer
import com.hjq.permissions.OnPermissionCallback
import com.hjq.permissions.Permission
import com.hjq.permissions.XXPermissions
import io.reactivex.Observable
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.functions.Consumer
import io.reactivex.functions.Function
import io.reactivex.schedulers.Schedulers
import java.io.File

class MainActivity : AppCompatActivity() {

    private val mInFile: File by lazy { File(this.cacheDir, "jile.mp4") }
    private val mOutFile: File = File(Environment.getExternalStorageDirectory(), "out.mp4")

    private lateinit var binding: ActivityMainBinding


    private lateinit var player: MusicPlayer

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)


        player = MusicPlayer()
        player.setDataSource(mInFile.absolutePath)

        player.setOnPlayerErrorListener(object : PlayerErrorListener {
            override fun onPlayerError(code: Int, msg: String) {
                Log.e("lei", msg)
            }

        })


        setContentView(binding.root)

    }

    fun compressVideo(view: View) {
        player.play()
//        XXPermissions.with(this)
//            // 申请单个权限
//            .permission(Permission.READ_EXTERNAL_STORAGE)
//            // 申请多个权限
//            .permission(Permission.WRITE_EXTERNAL_STORAGE)
//            // 设置权限请求拦截器（局部设置）
//            //.interceptor(new PermissionInterceptor())
//            // 设置不触发错误检测机制（局部设置）
//            //.unchecked()
//            .request(object : OnPermissionCallback {
//
//                override fun onGranted(permissions: MutableList<String>, all: Boolean) {
//                      if(all){
//                            compressVideo()
//                      }
//                }
//
//                override fun onDenied(permissions: MutableList<String>, never: Boolean) {
//
//                }
//            })

    }


    private fun compressVideo() {
//        val compressCommand =
//            arrayOf("ffmpeg", "-i", mInFile.absolutePath, "-b:v", "1024k", mOutFile.absolutePath)
//        Observable.just(compressCommand)
//            .map {
//                val videoCompress = VideoCompress()
//                videoCompress.compressVideo(compressCommand, object : CompressCallback {
//                    override fun onCompress(current: Int, total: Int) {
//                        Log.e("TAG", "压缩进度：$current/$total")
//                    }
//                })
//                mOutFile
//            }.subscribeOn(Schedulers.io())
//            .observeOn(AndroidSchedulers.mainThread())
//            .subscribe { // 压缩完成
//                Log.e("TAG", "压缩完成")
//            }
    }


//    companion object {
//        // Used to load the 'musicplay' library on application startup.
//        init {
//            System.loadLibrary("hellomusic")
//        }
//    }
}