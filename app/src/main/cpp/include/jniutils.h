#pragma once
#include <android/log.h>
#define TAG "myjni"

//#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)
//#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGE(...)
#define LOGD(...)

char* GetStaticStringField(JNIEnv *env,jobject thiz,const char* clazz,const char* fieldName){
    jclass jclazz = env->FindClass(clazz);
    jfieldID jSingleCodeID = env->GetStaticFieldID(jclazz, fieldName,"Ljava/lang/String;");
    jstring jSingleCode = (jstring) env->GetStaticObjectField(jclazz, jSingleCodeID);
    return const_cast<char *>(env->GetStringUTFChars(jSingleCode, JNI_FALSE));
}

void SetStaticStringField(JNIEnv *env,jobject thiz,const char* clazz,const char* fieldName,const char* value){
    jclass jclazz = env->FindClass(clazz);
    jfieldID _jfieldID = env->GetStaticFieldID(jclazz, fieldName,"Ljava/lang/String;");
    env->SetStaticObjectField(jclazz,_jfieldID,env->NewStringUTF(value));
}

//void CallStaticMethod(JNIEnv *env,jobject thiz,const char* clazz,const char* chazz,const char* methodName)

//绑定类和native方法
void JNI_Binding(JNIEnv* env, const char* clazz, const JNINativeMethod* methods, jint nMethods){
    jclass jclazz = env->FindClass(clazz);
    if (!clazz){
        LOGD("%s not found",clazz);
    }
    if(env->RegisterNatives(jclazz, methods, nMethods))
    {
        LOGD("method not found");
    }
}
