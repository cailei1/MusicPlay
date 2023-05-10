//
// Created by 蔡磊 on 2023/5/10.
//

#ifndef MUSICPLAY_CALL_JAVA_BACK_H
#define MUSICPLAY_CALL_JAVA_BACK_H
#include "jni.h"
class CallJavaBack {
public:
    JNIEnv *jniEnv;
    jobject musicPlayer;
public:
    void callJavaError(int code,char *msg) const;
    CallJavaBack();
    CallJavaBack(JNIEnv* env,jobject musicPlayer);
};


#endif //MUSICPLAY_CALL_JAVA_BACK_H
