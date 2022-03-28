#include <jni.h>
#include <string>
#include <unistd.h>
#include "loginUtils.h"
#include <cstring>
#include "include/jniutils.h"

using namespace std;
typedef struct{
    int x;
    int y;
}Soldier;

typedef struct{
    int id;
    int mapX;
    int mapY;
    float hp;
    int rayX;
    int rayY;
    int h;
    int distance;
    int cdd;
    int cdz;
    int cdzid;
    int hc;
}Hero;

typedef struct{
    Hero players[5];
    int buff[12];
    Soldier soldier[256];
    int num;
    int status;
}Data;

Data data;

char addr[64]={0};

char cmd[256];

int size_w,px,py;

void getResolvingPower() {
    std::string result = getCmdResult("su -c dumpsys display | grep mOverrideDisplayInfo");
    int pos = result.find("real");
    int x = 2340, y = 1080;
    if (pos != result.npos) {
        result = result.substr(pos + 5, 11);
        sscanf(result.c_str(), "%d x %d", &y, &x);
    } else {
        printf("Cannot get screen size,set to default!\n");
        std::string result1 = getCmdResult("wm size");
        sscanf(result1.c_str(), "Physical size: %ix%i", &y, &x);
    }
    size_w = (x > y) ? y : x;
    px = (x > y) ? x : y;
    py = (x < y) ? x : y;
    printf("ScreenSize : %i x %i\n", ((x > y) ? x : y), ((x < y) ? x : y));
}


//登录
jstring login(JNIEnv *env,jobject thiz){
    getResolvingPower();
    string statusCode;
    char* singleCode = GetStaticStringField(env,thiz,"com/agoda/umbrella/helper/ConfigHelper","singleCode");
    LOGD("%s",singleCode);
    char* version = GetStaticStringField(env,thiz,"com/agoda/umbrella/helper/ConfigHelper","version");
    char* mac = GetStaticStringField(env,thiz,"com/agoda/umbrella/helper/ConfigHelper","mac");
    statusCode = SingleCodeLogin(singleCode,mac,version);
    sprintf(cmd,"su -c 'echo %s %s %s %d %d %d> /sdcard/Android/data/com.tencent.tmgp.sgame/files/.key'",singleCode,version,mac,size_w,px,py);
    LOGD("%s",cmd);
    system(cmd);
    if (statusCode.length()==32){
        //获取地址
        sprintf(addr,"%ld",&data);
        //调用DataProvider.Init()
        jclass clazz1 = env->FindClass("com/agoda/umbrella/data/DataProvider");
        jmethodID methodID1 = env->GetStaticMethodID(clazz1,"Init","()V");
        env->CallStaticVoidMethod(clazz1,methodID1);

        //调用IconHelper.init()
        jclass clazz2 = env->FindClass("com/agoda/umbrella/helper/IconHelper");
        jmethodID methodID2 = env->GetStaticMethodID(clazz2,"init","()V");
        env->CallStaticVoidMethod(clazz2,methodID2);
    }

//    return env->NewStringUTF(addr);
    return env->NewStringUTF(statusCode.c_str());
}

jstring getKey(JNIEnv *env,jobject thiz) {
    LOGD("addr = %ld",addr);
    return env->NewStringUTF(addr);
}

void inject(JNIEnv *env,jobject thiz, jstring filesPath){
    //修改crc32
    jclass clazz = env->FindClass("com/agoda/umbrella/utils/Crc32Editor");
    jmethodID methodID = env->GetStaticMethodID(clazz,"editFile","(Ljava/lang/String;Ljava/lang/String;)Z");
    env->CallStaticBooleanMethod(clazz,methodID,env->NewStringUTF("part3"),env->NewStringUTF("ef53afe4"));

    //替换so
    string apkPath = getCmdResult("su -c 'pm path com.tencent.tmgp.sgame'");

    sprintf(cmd,"su -c 'cp %s/part3 %slib/arm64/libspecialhttpdns.so'",env->GetStringUTFChars(filesPath,nullptr),apkPath.substr(8,apkPath.length()-16).c_str());
    system(cmd);
    LOGE("%s",cmd);
    sprintf(cmd,"su -c 'chmod 755 %slib/arm64/libspecialhttpdns.so'",apkPath.substr(8,apkPath.length()-16).c_str());
    system(cmd);

}

void clean(JNIEnv *env,jobject thiz,jstring filesPath){
    string apkPath = getCmdResult("su -c 'pm path com.tencent.tmgp.sgame'");
    sprintf(cmd,"su -c 'cp %s/part4 %slib/arm64/libspecialhttpdns.so'",env->GetStringUTFChars(filesPath,nullptr),apkPath.substr(8,apkPath.length()-16).c_str());
    LOGE("%s",cmd);
    system(cmd);
    sprintf(cmd,"su -c 'chmod 755 %slib/arm64/libspecialhttpdns.so'",apkPath.substr(8,apkPath.length()-16).c_str());
    system(cmd);
}

//设置数据
void setData(JNIEnv *env,jobject thiz){
    //读取结构体
    jclass classDataParser = env->FindClass("com/agoda/umbrella/data/DataParser");
    //绘制状态
    jfieldID statusID = env->GetStaticFieldID(classDataParser, "status", "I");
    env->SetStaticIntField(classDataParser,statusID,data.status);
    LOGD("jni status = %d",data.status);
    //LOGE("status = %d",data.status);
    if(data.status == 0){
        sleep(1);
        return;
    }
    //英雄
    jfieldID heroArrayID = env->GetStaticFieldID(classDataParser, "heroArray", "[Lcom/agoda/umbrella/data/DataParser$Hero;");
    auto heroArray = (jobjectArray)env->GetStaticObjectField(classDataParser, heroArrayID);
    jclass classHero = env->FindClass("com/agoda/umbrella/data/DataParser$Hero");
    jmethodID heroConstruct = env->GetMethodID(classHero, "<init>", "()V");
    jfieldID id = env->GetFieldID(classHero,"id", "I");
    jfieldID mapX = env->GetFieldID(classHero,"mapX", "I");
    jfieldID mapY = env->GetFieldID(classHero,"mapY", "I");
    jfieldID hp = env->GetFieldID(classHero,"hp", "F");
    jfieldID cdd = env->GetFieldID(classHero,"cdd", "I");
    jfieldID cdz = env->GetFieldID(classHero,"cdz", "I");
    jfieldID cdzid = env->GetFieldID(classHero,"cdzid", "I");
    jfieldID rayX = env->GetFieldID(classHero,"rayX", "I");
    jfieldID rayY = env->GetFieldID(classHero,"rayY", "I");
    jfieldID h = env->GetFieldID(classHero,"h", "I");
    jfieldID distance = env->GetFieldID(classHero,"distance", "I");
    jfieldID hc = env->GetFieldID(classHero,"hc", "I");
    for (int i = 0; i < 5; i++) {
        jobject hero  = env->NewObject(classHero,heroConstruct);
        env->SetIntField(hero,id,(data.players)[i].id);
        env->SetIntField(hero,mapX,(data.players)[i].mapX);
        env->SetIntField(hero,mapY,(data.players)[i].mapY);
        env->SetFloatField(hero,hp,(data.players)[i].hp);
        env->SetIntField(hero,cdd,(data.players)[i].cdd);
        env->SetIntField(hero,cdz,(data.players)[i].cdz);
        env->SetIntField(hero,cdzid,(data.players)[i].cdzid);
        env->SetIntField(hero,rayX,(data.players)[i].rayX);
        env->SetIntField(hero,rayY,(data.players)[i].rayY);
        env->SetIntField(hero,h,(data.players)[i].h);
        env->SetIntField(hero,distance,(data.players)[i].distance);
        env->SetIntField(hero,hc,(data.players)[i].hc);
        env->SetObjectArrayElement(heroArray,i,hero);
//        if((data.players)[i].rayX == 0 ){
//            LOGD(" id = %d",(data.players)[i].id);
//            LOGD("**************");
//            LOGD("*** id = %d",(data.players)[i].id);
//            LOGD("*** mapX = %d",(data.players)[i].mapX);
//            LOGD("*** mapY = %d", (data.players)[i].mapY);
//            LOGD("*** rayX = %d",(data.players)[i].rayX);
//            LOGD("*** rayY = %d", (data.players)[i].rayY);
//            LOGD("*** distance = %d", (data.players)[i].distance);
//            LOGD("**************");
//        }

        env->DeleteLocalRef(hero);
    }
    //野怪
    jfieldID buffArrayID = env->GetStaticFieldID(classDataParser, "buffArray", "[Lcom/agoda/umbrella/data/DataParser$Buff;");
    jobjectArray buffArray = (jobjectArray)env->GetStaticObjectField(classDataParser, buffArrayID);
    jclass classBuff = env->FindClass("com/agoda/umbrella/data/DataParser$Buff");
    jmethodID buffConstruct = env->GetMethodID(classBuff, "<init>", "()V");
    jfieldID cd = env->GetFieldID(classBuff,"cd", "I");
    for (int i = 0; i < 12; i++) {
        jobject buff  = env->NewObject(classBuff,buffConstruct);
        env->SetIntField(buff,cd,data.buff[i]);
        env->SetObjectArrayElement(buffArray,i,buff);
        env->DeleteLocalRef(buff);
    }

    //兵线
    jfieldID soldierArrayID = env->GetStaticFieldID(classDataParser, "soldierArray", "[Lcom/agoda/umbrella/data/DataParser$Soldier;");
    auto soldierArray = (jobjectArray)env->GetStaticObjectField(classDataParser, soldierArrayID);
    jclass classSoldier = env->FindClass("com/agoda/umbrella/data/DataParser$Soldier");
    jmethodID soldierConstruct = env->GetMethodID(classSoldier, "<init>", "()V");
    jfieldID sMapX = env->GetFieldID(classSoldier,"mapX", "I");
    jfieldID sMapY = env->GetFieldID(classSoldier,"mapY", "I");
    LOGD("num = %d",data.num);
    for (int i = 0; i < data.num; i++) {
        jobject soldier  = env->NewObject(classSoldier,soldierConstruct);
        env->SetIntField(soldier,sMapX,(data.soldier[i]).x);
        env->SetIntField(soldier,sMapY,(data.soldier[i]).y);
        LOGD("%d-%d",(data.soldier[i]).x,(data.soldier[i]).y);
        env->SetObjectArrayElement(soldierArray,i,soldier);
        env->DeleteLocalRef(soldier);
    }

    //兵线数量
    jfieldID numID = env->GetStaticFieldID(classDataParser, "num", "I");
    env->SetStaticIntField(classDataParser,numID,data.num);


}

//SplashActivity native方法数组
static const JNINativeMethod methodsMainActivity[] = {
        {"o0o0oo0o0", "()Ljava/lang/String;", (void*)login},
        {"oo0000oo0", "()Ljava/lang/String;", (void*)getKey},

};

//DrawView native方法数组
static const JNINativeMethod methodsDrawView[] = {
        {"o00o0ooo0", "()V", (void*)setData}
};

//SettingFragment native方法数组
static const JNINativeMethod methodsCoreFragment[] = {
        {"o0o0000oo0", "(Ljava/lang/String;)V", (void*)inject},
        {"oo0000ooo0", "(Ljava/lang/String;)V", (void*)clean}
};


//动态注册
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, __unused void* reserved){

    JNIEnv* env = nullptr;
    if(vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
        return -1;
    JNI_Binding(env,"com/agoda/umbrella/activity/LoginActivity",methodsMainActivity,sizeof(methodsMainActivity)/sizeof(methodsMainActivity[0]));
    JNI_Binding(env, "com/agoda/umbrella/view/DrawView", methodsDrawView,sizeof(methodsDrawView) / sizeof(methodsDrawView[0]));
    JNI_Binding(env, "com/agoda/umbrella/fragment/CoreFragment", methodsCoreFragment,sizeof(methodsCoreFragment) / sizeof(methodsCoreFragment[0]));

    return JNI_VERSION_1_4;
}