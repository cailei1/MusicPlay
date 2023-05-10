//
// Created by 蔡磊 on 2023/5/10.
//

#include "AudioTrackPlayer.h"


jobject AudioTrackPlayer::createAudioTrack() {
    //    public AudioTrack(int streamType, int sampleRateInHz, int channelConfig, int audioFormat,
//            int bufferSizeInBytes, int mode, int sessionId)

    jclass jAudioTrackClass = jniEnv->FindClass("android/media/AudioTrack");
    jmethodID jAudioTrackMid = jniEnv->GetMethodID(jAudioTrackClass, "<init>", "(IIIIII)V");
    int streamType = 3;
    int sampleRateInHz = 44100;
    int channelConfig = (0x4 | 0x8);
    int audioFormat = 2;

    //  static public int getMinBufferSize(int sampleRateInHz, int channelConfig, int audioFormat)
    jmethodID getMinBufferSizeMid = jniEnv->GetStaticMethodID(jAudioTrackClass, "getMinBufferSize",
                                                              "(III)I");

    int bufferSizeInBytes = jniEnv->CallStaticIntMethod(jAudioTrackClass, getMinBufferSizeMid,
                                                        sampleRateInHz, channelConfig, audioFormat);
    int mode = 1;
    audioTracker = jniEnv->NewObject(jAudioTrackClass, jAudioTrackMid, streamType,
                                               sampleRateInHz, channelConfig, audioFormat,
                                               bufferSizeInBytes, mode);

    jmethodID playMid = jniEnv->GetMethodID(jAudioTrackClass, "play", "()V");
    audioWriteMid = jniEnv->GetMethodID(jAudioTrackClass, "write", "([BII)I");
    jniEnv->CallVoidMethod(audioTracker, playMid);
    return audioTracker;
}

AudioTrackPlayer::AudioTrackPlayer(JavaVM *javaVm, JNIEnv *env) {
    this->javaVm = javaVm;
    this->jniEnv = env;
    createAudioTrack();
}

AudioTrackPlayer::~AudioTrackPlayer() {
    jniEnv->DeleteLocalRef(audioTracker);
}

void
AudioTrackPlayer::callAudioTrackWrite(jbyteArray audioData, int offsetInBytes, int sizeInBytes) {
    jniEnv->CallIntMethod(audioTracker, audioWriteMid, audioData, offsetInBytes, sizeInBytes);
}


