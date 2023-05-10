//
// Created by 蔡磊 on 2023/5/10.
//

#ifndef MUSICPLAY_CLFFMPEG_H
#define MUSICPLAY_CLFFMPEG_H
extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "jni.h"
#include "AudioTrackPlayer.h"
#include "call_java_back.h"
};

class CLFFmpeg {
public:

    AVCodecContext *avCodecContext = nullptr;
    AVFormatContext *avFormatContext = nullptr;
    const char* filePath;
    JNIEnv* jniEnv;

    SwrContext *swrContext = nullptr;

    jbyteArray jPcmByteArray;
    jbyte *jPcmData;
    int dataSize;

    int audioStreamIndex;

    AVPacket *avPacket;
    AVFrame *pFrame;

    AudioTrackPlayer *audioTrackPlayer;

    CallJavaBack callJavaBack;


public:
    CLFFmpeg(AudioTrackPlayer *audioTrackPlayer,JNIEnv *env,const char *filePath,CallJavaBack callback);
    ~CLFFmpeg();

public:
    void openInput();

    void findAudioStream();

    void findAudioCodec();

    void swrSamplePrepare();

    void decodeAndPlayAudio();



};


#endif //MUSICPLAY_CLFFMPEG_H
