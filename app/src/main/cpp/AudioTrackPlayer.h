//
// Created by 蔡磊 on 2023/5/10.
//


#ifndef MUSICPLAY_AUDIOTRACKPLAYER_H
#define MUSICPLAY_AUDIOTRACKPLAYER_H

#include "jni.h"
#include "string.h"


class AudioTrackPlayer {
public:
    jobject audioTracker;
    jmethodID audioWriteMid;
    JNIEnv *jniEnv;
    JavaVM *javaVm;
public:
    AudioTrackPlayer(JavaVM *javaVm,JNIEnv *env);
    ~AudioTrackPlayer();
public:
    jobject createAudioTrack();
    void callAudioTrackWrite(jbyteArray audioData,int offsetInBytes,int sizeInBytes);
};


#endif //MUSICPLAY_AUDIOTRACKPLAYER_H
