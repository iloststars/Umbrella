#include <stdio.h>
#include <string>
#include <vector>
#include <regex>
#include "urlencoding.h"
#include "rc4.h"
#include "httpUtils.h"
#include "print.h"
#include <android/log.h>
using namespace std;

#define LOG_TAG    "Beacon"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
//#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGE(...)
#define LOGD(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

string getCmdResult(const string &strCmd) {
    char buf[10240] = {0};
    FILE *pf = NULL;
    if ((pf = popen(strCmd.c_str(), "r")) == NULL) {
        return "";
    }
    string strResult;
    while (fgets(buf, sizeof buf, pf)) {
        strResult += buf;
    }
    pclose(pf);
    unsigned int iSize = strResult.size();
    if (iSize > 0 && strResult[iSize - 1] == '\n') {
        strResult = strResult.substr(0, iSize - 1);
    }
    return strResult;
}

string getUUID() {
    string uuid = getCmdResult("getprop ro.serialno");
    return uuid;
}

void rc4_send2(char raw_data[],char *after_decode){
    char key[] = "YJWVfgCBz43GXZX8TS";
    string url = "https://cms.dingtuoos.com/AppEn.php?appid=1000001&m=d8d901bef932565e0c1823cf6ed5ad35";
    char tmp1[64]="";
    char tmp[64]="";
    time_t t = time(0);
    strftime(tmp, sizeof(tmp), "%H",localtime(&t));
    char usedkey[200]="";
    strcat(usedkey,key);
    strcat(usedkey,tmp);
    strftime(tmp1, sizeof(tmp1), "%d",localtime(&t));
    strcat(usedkey,tmp1);
    char after_rc4[1024]={0};
    LOGE("Raw:\t\t\t%s",raw_data);
    EncodeRC4Base64(raw_data,after_rc4,usedkey);
    LOGE("After RC4:\t\t%s",after_rc4);
    urlencode(after_rc4);
    LOGE("After URLencode:\t%s",after_rc4);
    char msg[1024];
    sprintf(msg,"parameter=%s",after_rc4);
    std::string backstring = post(url,msg, true);
    LOGE("Rec:\t\t\t%s",backstring.c_str());
    DecodeBase64RC4(backstring.c_str(),after_decode,usedkey);
    LOGE("After Decode:\t%s",after_decode);
}

//静态
void rc4_send(char raw_data[],char *after_decode){
    char key[] = {'o','T','B','m','I','M','6','u','m','p','D','Q','g','n','3','y','9','h','\0'};//RC4 key
    //bilibli
    char url[] = {'h','t','t','p','s',':','/','/','c','.','d','i','n','g','t','u','o','o','s','.','c','o','m','/','A','p','p','E','n','.','p','h','p','?','a','p','p','i','d','=','1','&','m','=','a','d','6','9','3','e','9','0','e','7','2','e','8','c','9','2','6','8','1','a','d','0','9','2','3','9','e','6','2','3','4','5','\0'};

    //ghost
//    char url[] = {'h','t','t','p','s',':','/','/','c','.','d','i','n','g','t','u','o','o','s','.','c','o','m','/','A','p','p','E','n','.','p','h','p','?','a','p','p','i','d','=','2','&','m','=','7','6','0','0','4','3','1','2','7','a','5','4','1','4','6','b','1','5','6','0','4','8','1','3','6','f','4','f','c','b','a','6','\0'};

    char after_rc4[1024]={0};
    LOGE("------------------------------------------------------");
    LOGE("Raw:\t\t\t%s",raw_data);
    EncodeRC4Base64(raw_data,after_rc4,key);
    LOGE("After RC4:\t\t%s",after_rc4);
    urlencode(after_rc4);
    LOGE("After URLencode:\t%s",after_rc4);
    char msg[1024];
    sprintf(msg,"parameter=%s",after_rc4);
    string backstring = post(url,msg, true);
//    LOGE("Rec:\t\t\t%s",backstring.c_str());
    DecodeBase64RC4(backstring.c_str(),after_decode,key);
//    LOGE("After Decode:\t%s",after_decode);
}

//动态
//void rc4_send(char raw_data[],char *after_decode){
//    char key[] = {'Y','J','W','V','f','g','C','B','z','4','3','G','X','Z','X','8','T','S','\0'}; //RC4 key
//    char url[] = {'h','t','t','p','s',':','/','/','c','m','s','.','d','i','n','g','t','u','o','o','s','.','c','o','m','/','A','p','p','E','n','.','p','h','p','?','a','p','p','i','d','=','1','&','m','=','a','d','6','9','3','e','9','0','e','7','2','e','8','c','9','2','6','8','1','a','d','0','9','2','3','9','e','6','2','3','4','5','\0'};
//    char after_rc4[1024]={0};
//    char tmp1[64]="";
//    char tmp[64]="";
//    time_t t = time(0);
//    strftime(tmp, sizeof(tmp), "%H",localtime(&t));
//    char usedkey[200]="";
//    strcat(usedkey,key);
//    strcat(usedkey,tmp);
//    strftime(tmp1, sizeof(tmp1), "%d",localtime(&t));
//    strcat(usedkey,tmp1);
//    print("------------------------------------------------------");
//    print("Raw:\t\t\t%s",raw_data);
//    EncodeRC4Base64(raw_data,after_rc4,usedkey);
//    print("After RC4:\t\t%s",after_rc4);
//    urlencode(after_rc4);
//    print("After URLencode:\t%s",after_rc4);
//    char msg[1024];
//    sprintf(msg,"parameter=%s",after_rc4);
//    std::string backstring = post(url,msg, true);
//    print("Rec:\t\t\t%s",backstring.c_str());
//    DecodeBase64RC4(backstring.c_str(),after_decode,usedkey);
//    print("After Decode:\t%s",after_decode);
//}

std::vector<string> split(string text,const char* regex)
{
    std::regex ws_re(regex);  // white space
    return vector<string>(sregex_token_iterator(text.begin(), text.end(), ws_re, -1), sregex_token_iterator());
}


string singleLogin(string singleCode,string deviceid){
    char after_decode[1024];
    char raw_data[1024];
    //bilibli Balloon-pro
    sprintf(raw_data,"&api=login.ic&mutualkey=69e92cc4e8518841c52df6fc6cd60593&BSphpSeSsL=asdfas&"
                     "icid=%s&key=%s&maxoror=%s",singleCode.c_str(),deviceid.c_str(),deviceid.c_str());
    //ghost
//    sprintf(raw_data,"&api=login.ic&mutualkey=3c2ebc65a57ed391fd39d3baff89169e&BSphpSeSsL=asdfas&"
//                     "icid=%s&key=%s&maxoror=%s",singleCode.c_str(),deviceid.c_str(),deviceid.c_str());
    rc4_send(raw_data,after_decode);
    return after_decode;
}

int getSocketPort(){
    unsigned long num = 0;
    char *pch;
    char line[64];
    FILE *fp = fopen("/proc/sys/kernel/random/boot_id","r");
    if (fp != nullptr) {
        fgets(line, sizeof(line), fp);
        pch = strtok(line, "-");
        num = strtoul(pch, nullptr, 16);
        fclose(fp);
        return (int)(num % 2233 + 60000);
    }
    return 59438;
}

string userLogin(string username,string password,string version,string deviceid){
    char after_decode[1024];
    char raw_data[1024];
    char mutualkey[] = "d1b98582e9fa50a104ef6fdf12d5a39f";
    sprintf(raw_data,"&api=login.lg&mutualkey=%s&appsafecode=%d&BSphpSeSsL=asdfas&"
                     "user=%s&pwd=%s&key=%s&maxoror=%s",mutualkey,getSocketPort(),username.c_str(),password.c_str(),deviceid.c_str(),deviceid.c_str());
    rc4_send2(raw_data,after_decode);
    return after_decode;
}


