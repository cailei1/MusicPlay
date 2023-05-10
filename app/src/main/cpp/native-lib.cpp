#include <jni.h>
#include <string>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include <libavutil/time.h>
#include "AudioTrackPlayer.h"
#include "CLFFmpeg.h"
char *testPrint(char *said);
}

#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_INFO,"lei",__VA_ARGS__)


extern "C" JNIEXPORT jstring JNICALL
Java_com_cailei_musicplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

int ffmpegmain(int argc, char **argv, void(call_back)(int, int));


extern "C"
JNIEXPORT void JNICALL
Java_com_cailei_musicplay_VideoCompress_compressVideo(JNIEnv *env, jobject thiz,
                                                      jobjectArray compress_command,
                                                      jobject compress_callback) {


    // ffmpeg 处理视频压缩
    int argCount = env->GetArrayLength(compress_command);
    char **argv = (char **) malloc(sizeof(char *) * argCount);
    for (int i = 0; i < argCount; ++i) {
        jstring j_param = static_cast<jstring>(env->GetObjectArrayElement(compress_command, i));
        argv[i] = (char *) env->GetStringUTFChars(j_param, NULL);
    }
    // 调用命令去压缩
//    ffmpegmain(argCount, argv, NULL);
//    av_write_trailer(NULL);
    LOGD("fjsd;fja;sdfas;df;ashf");
    LOGD("%s", testPrint("world"));
    // 释放内存
    for (int i = 0; i < argCount; ++i) {
        free(argv[i]);
    }
    free(argv);

//    ffmpegmain()

}

extern "C"
JNIEXPORT void JNICALL
Java_com_cailei_musicplay_music_MusicPlayer_nPlay(JNIEnv *env, jobject thiz, jstring url) {

    const char *filePath = env->GetStringUTFChars(url, 0);

    AudioTrackPlayer *audioTrackPlayer = new AudioTrackPlayer(NULL, env);

    CallJavaBack callJavaBack(env,thiz);

    CLFFmpeg *clfFmpeg = new CLFFmpeg(audioTrackPlayer, env, filePath,callJavaBack);
    // 打开文件
    clfFmpeg->openInput();
    clfFmpeg->findAudioStream();
    // 查找解码器
    clfFmpeg->findAudioCodec();
    // 重采样 end
    clfFmpeg->swrSamplePrepare();

    // 解码并播放
    clfFmpeg->decodeAndPlayAudio();
}