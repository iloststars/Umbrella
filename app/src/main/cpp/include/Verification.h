#pragma once
#include "print.h"
#include "HttpReq.h"
void substring( const char *s, char ch1, char ch2, char *substr )   //分割字符串
{
    while( *s && *s++!=ch1 ) ;
    while( *s && *s!=ch2 ) *substr++=*s++ ;
    *substr='\0';
}


const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

char get_index(char ch)
{
    char *ptr = (char*)strrchr(base, ch);
    return (ptr - base);
}

//RC4
void RC4(unsigned char *pSrc, int nSrcLen, const char *pKey, int nKeyLen);

//base64->RC4
int DecodeBase64RC4(const char* pSrc, char* pDst, const char* pKey);



void RC4(unsigned char *pSrc, int nSrcLen, const char *pKey, int nKeyLen)
{
    const int MAX_SIZE = 10*1024;
    unsigned char S[256] = {0}, T[256] = {0}, R[MAX_SIZE] = {0};

    if(!pSrc || !pKey || nSrcLen <=0 || nSrcLen > MAX_SIZE || nKeyLen <=0)
        return;

    int i = 0;
    for(i = 0; i < 256; i++)
    {
        S[i] = i;
        T[i] = pKey[i%nKeyLen];
    }

    int j = 0;
    for(i = 0; i < 256; i++)
    {
        j = (j+S[i]+T[i])%256;

        unsigned char temp = S[i];
        S[i] = S[j], S[j] = temp;
    }

    i=0, j=0;
    for(int m = 0; m < MAX_SIZE; m++)
    {
        i = (i+1)%256;
        j = (j+S[i])%256;

        unsigned char temp = S[i];
        S[i] = S[j], S[j] = temp;

        int t = (S[i]+S[j])%256;
        R[m] = S[t];
    }

    for(i = 0; i < nSrcLen; i++)
    {
        pSrc[i] ^= R[i];
    }
}

void DecodeString2Binary(const char *src, int lenSrc, char* &res, int &lenRes)
{
    lenRes = lenSrc*3/4;
    res = new char[lenRes];

    int cp = 0;
    if(src[lenSrc-1] == '='){lenRes--, cp++;}
    if(src[lenSrc-2] == '='){lenRes--, cp++;}

    char* newSrc = new char[lenSrc-cp];
    int i;
    for(i = 0; i<lenSrc-cp; i++)
        newSrc[i] = get_index(src[i]);

    for(i= 0; i< lenRes; i+=3)
    {
        res[i] = (newSrc[i/3*4]<<2) + (newSrc[i/3*4 + 1]>>4);
        res[i+1] = ((newSrc[i/3*4 + 1]&15) <<4) + (newSrc[i/3*4 + 2]>>2);
        res[i+2] = ((newSrc[i/3*4 + 2]&3) <<6) + (newSrc[i/3*4 + 3]);
    }

    delete []newSrc;
    return;
}



int DecodeBase64RC4(const char* pSrc, char* pDst, const char* pKey)
{
    if(!pSrc || !pDst || !pKey)
        return 0;

    int nSrcLen = strlen(pSrc);
    int nKeyLen = strlen(pKey);

    int nResLen = 0;
    char *pRes = NULL;
    DecodeString2Binary(pSrc, nSrcLen, pRes, nResLen);

    if(pRes)
    {
        RC4((unsigned char*)pRes, nResLen, pKey, nKeyLen);

        memcpy(pDst, pRes, nResLen);
        pDst[nResLen] = '\0';

        delete []pRes, pRes = NULL;
        return nResLen;
    }
    return 0;
}

char res[200];

string httpPost(string url, string data)
{
    HttpRequest *Http;
    char retDate[256];
    char http_return[4096] = { 0 };
    char text[4096] = { 0 };
    if (Http->HttpPost(url.c_str(), data.c_str(), http_return))
    {
        sprintf(text, "%s", http_return);
        char res[64] = "\n\r.+$";
        getText(http_return, retDate, res);
        sscanf(retDate,"\n\r%s",retDate);
    }
    return retDate;
}

//char core_data[1024] = { 0 };  //响应数据
const char *core_data;//响应数据
const char* getCoreData(string singleCode, string device_id, string version) {
    //这里的post函数自行替换即可
//    sprintf(data, "num=1&SingleCode=%s&ver=%s&mac=%s", singleCode.c_str(),version.c_str(),device_id.c_str());
//    httpPost("http://www.cngayhub.icu:88/Single.php", data, core_data);
//    core_data = httpPost("http://www.cngayhub.icu:88/Single.php", "num=1&SingleCode="+singleCode+"&ver="+version+"&mac="+device_id).c_str();
//    core_data = post("http://www.cngayhub.icu:88/Single.php", "num=1&SingleCode="+singleCode+"&ver="+version+"&mac="+device_id,false).c_str();//Ballon
//    core_data = post("http://www.cngayhub.icu:88/Single.php", "num=3&SingleCode="+singleCode+"&ver="+version+"&mac="+device_id,false).c_str();//Star
//    core_data = post("http://www.cngayhub.icu:88/Single.php", "num=4&SingleCode="+singleCode+"&ver="+version+"&mac="+device_id,false).c_str();//Sky
//    core_data = post("http://www.cngayhub.icu:88/Single.php", "num=5&SingleCode="+singleCode+"&ver="+version+"&mac="+device_id,false).c_str();//Ghost
    core_data = post("http://www.cngayhub.icu:88/Single.php", "num=6&SingleCode="+singleCode+"&ver="+version+"&mac="+device_id,false).c_str();//V10
//    core_data = post("http://www.cngayhub.icu:88/Single.php", "num=7&SingleCode="+singleCode+"&ver="+version+"&mac="+device_id,false).c_str();//Xie
//    core_data = post("http://www.cngayhub.icu:88/Single.php", "num=8&SingleCode="+singleCode+"&ver="+version+"&mac="+device_id,false).c_str();//baishi
    print("coredata = %s",core_data);
    substring( core_data, '{', '}', res );
    print("未解密数据:%s\n", res);

    char res2[128] = {0};
    time_t t = time(0);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%M",localtime(&t) );
    char jc1[128]="hfhguihrungehr843g";        //默认固定密钥
    char jc2[128]="1584dfasdfewfdsvrea";        //默认固定密钥
    char key[80];
    memset(key, '\0', sizeof(key));
    strcpy(key,jc1);
    strcat(key,tmp);
    strcat(key,jc2);
    DecodeBase64RC4(res, res2,key);
    if(res2[0]!='D'){
        int min=atoi(tmp);
        sprintf(tmp,"%d",min-1);
        memset(key, '\0', sizeof(key));
        strcpy(key,jc1);
        strcat(key,tmp);
        strcat(key,jc2);
        DecodeBase64RC4(res, res2,key);
    }
    print("密钥为:%s",key);
    if(res2[0]!='D'){print("解密异常");exit(0);}
    strcpy((char*)res2,(char*)res2+1);
    print("解密后:%s",res2);
    return res2;
}


