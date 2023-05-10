//
// Created by 蔡磊 on 2023/5/10.
//

#include "call_java_back.h"

void CallJavaBack::callJavaError(int code, char *msg) const {
     jclass musicPlayerClass = jniEnv->FindClass("com/cailei/musicplay/music/MusicPlayer");
     jmethodID musicMethodId = jniEnv->GetMethodID(musicPlayerClass,"onJniCallError","(ILjava/lang/String;)V");
     jstring sMsg = jniEnv->NewStringUTF(msg);
     jniEnv->CallVoidMethod(musicPlayer,musicMethodId,code,sMsg);
     jniEnv->DeleteLocalRef(sMsg);
}

CallJavaBack::CallJavaBack(JNIEnv *env,jobject musicPlayer) {
  this->jniEnv = env;
  this->musicPlayer = musicPlayer;
}

CallJavaBack::CallJavaBack() {

}
