//
// Created by 蔡磊 on 2023/5/10.
//

#include "CLFFmpeg.h"
#include "android/log.h"
#define LOGD(...) __android_log_print(ANDROID_LOG_INFO,"lei",__VA_ARGS__)

void CLFFmpeg::openInput() {
    av_register_all();
    avformat_network_init();

    int ret;
//    LOGD("find stream failed %s", filePath);

    avFormatContext = avformat_alloc_context();

    ret = avformat_open_input(&avFormatContext, filePath, NULL, NULL);

    if (ret) {
        // 不成功 ，1.回调给java层 通知问题 2.需要释放资源
//        goto __av_resources_destory;
//        LOGD("find stream failed %s", av_err2str(ret));
    }

}

CLFFmpeg::CLFFmpeg(AudioTrackPlayer *audioTrackPlayer, JNIEnv *env, const char *filePath,CallJavaBack callJavaBack) {
    this->jniEnv = env;
    this->filePath = filePath;
    this->audioTrackPlayer = audioTrackPlayer;
    this->callJavaBack = callJavaBack;
}

void CLFFmpeg::findAudioStream() {
    int ret = avformat_find_stream_info(avFormatContext, nullptr);

    if (ret < 0) {
//        LOGD("find stream failed %s", av_err2str(ret));
    }

    // 找到音频流的index
    audioStreamIndex = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);

    if (audioStreamIndex < 0) {

    }
}

void CLFFmpeg::findAudioCodec() {
    AVCodecParameters *avCodecParameters = avFormatContext->streams[audioStreamIndex]->codecpar;
    AVCodec *avCodec = avcodec_find_decoder(avCodecParameters->codec_id);


    if (avCodec == nullptr) {
        LOGD("find stream failed %s");
        return;
    }

    avCodecContext = avcodec_alloc_context3(avCodec);

    avcodec_parameters_to_context(avCodecContext, avCodecParameters);

//    LOGD("find stream failed %s", av_err2str(ret));
    // 打开解码器
    int avCodecOpenRes = avcodec_open2(avCodecContext, avCodec, nullptr);

    if (avCodecOpenRes < 0) {
        LOGD("find stream failed %s", av_err2str(avCodecOpenRes));
    }
//    LOGD("find stream failed %s", av_err2str(avCodecOpenRes));
}

void CLFFmpeg::swrSamplePrepare() {
    // 重采样 start
    // int64_t out_ch_layout, enum AVSampleFormat out_sample_fmt, int out_sample_rate,
    //                                      int64_t  in_ch_layout, enum AVSampleFormat  in_sample_fmt, int  in_sample_rate,
    int64_t out_ch_layout = AV_CH_LAYOUT_STEREO;
    enum AVSampleFormat out_sample_fmt = AVSampleFormat::AV_SAMPLE_FMT_S16;
    int out_sample_rate = 44100;

    auto in_ch_layout = (int64_t) avCodecContext->channel_layout;
    enum AVSampleFormat in_sample_fmt = avCodecContext->sample_fmt;
    int in_sample_rate = avCodecContext->sample_rate;

    swrContext = swr_alloc();
    swr_alloc_set_opts(swrContext, out_ch_layout, out_sample_fmt, out_sample_rate,
                       in_ch_layout, in_sample_fmt, in_sample_rate, 0,
                       nullptr);

    int swrInitRes = swr_init(swrContext);

    if (swrContext == nullptr) {
//        LOGD("find stream failed swrContext");
        return;
    }
    if (swrInitRes < 0) {
        return;
    }

    int out_channels = av_get_channel_layout_nb_channels(out_ch_layout);
    dataSize = av_samples_get_buffer_size(nullptr, out_channels, avCodecContext->frame_size,
                                          out_sample_fmt, 1);
    jPcmByteArray = jniEnv->NewByteArray(dataSize);
    jPcmData = jniEnv->GetByteArrayElements(jPcmByteArray, NULL);
}

void CLFFmpeg::decodeAndPlayAudio() {
    avPacket = av_packet_alloc();
    pFrame = av_frame_alloc();
//    pFrame->format = avCodecParameters->format;
//    pFrame->channel_layout = avCodecParameters->channel_layout;
//    pFrame->nb_samples = avCodecParameters->frame_size; // 单通道每秒采样数
//    av_frame_get_buffer(pFrame, 0);


    auto *resampleOutBuffer = (uint8_t *) (malloc(dataSize));

    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        // packet 包，压缩数据 解码成pcm 数据
        if (avPacket->stream_index == audioStreamIndex) {
            int codeSendPacketRes = avcodec_send_packet(avCodecContext, avPacket);
            if (codeSendPacketRes == 0) {
                int codeReceiveFrameRes = avcodec_receive_frame(avCodecContext, pFrame);
                LOGD("解码结果%d ，", codeReceiveFrameRes);
                if (codeReceiveFrameRes >= 0) {
                    callJavaBack.callJavaError(1,"回调解码");
                    LOGD("解码 帧，一帧大小是 %d",pFrame->sample_rate);
                    // write java 的buffer
                    int swrRes = swr_convert(swrContext, &resampleOutBuffer, pFrame->nb_samples * 2,
                                             (const uint8_t **) (pFrame->data), pFrame->nb_samples);
                    if (swrRes < 0) {

                    }
                    memcpy(jPcmData, resampleOutBuffer, dataSize);
                    // 把c 的数组的数据同步到jbytearray,然后释放native 数组
                    jniEnv->ReleaseByteArrayElements(jPcmByteArray, jPcmData, JNI_COMMIT);
                    audioTrackPlayer->callAudioTrackWrite(jPcmByteArray, 0, dataSize);
//                env->DeleteLocalRef(jPcmByteArray);
                }
//                av_packet_unref(avPacket);
//                av_frame_unref(pFrame);
            }
        }
    }
    delete this;

}

CLFFmpeg::~CLFFmpeg() {
    av_packet_free(&avPacket);
    av_frame_free(&pFrame);
//    jniEnv->ReleaseByteArrayElements(jPcmByteArray, jPcmData, 0);
//    jniEnv->DeleteLocalRef(jPcmByteArray);
//    jniEnv->DeleteLocalRef(audioTrackPlayer->audioTracker);
    avformat_network_deinit();
    delete audioTrackPlayer;
}


