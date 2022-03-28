#pragma once
#include <vector>
#include <regex>
#include "httpUtils.h"
#include "Verification.h"
#include "print.h"

#define url_login "https://w.eydata.net/gj4tgfdg1a1ob9fe"
#define url_data "https://w.eydata.net/58gdf85e2a1ob9fe"
#define url_variable "https://w.eydata.net/3jf894jkj1a1ob9fe"



//string doLogin(const string& card, const string& device_id, const string& version) {
//    print("card = %s version = %s ",card.c_str(),version.c_str());
//    string url_login =  "https://w.eydata.net/ba594dd7ad7c2382";
//
//    string result = post(url_login, "Ver=" + version + "&Mac=" + device_id + "&SingleCode=" + card);
//    print("Login:%s",result.c_str());
//    return result;
//}

string SingleCodeLogin(string card, string device_id, string version) {
    const static string url1 = "https://w.eydata.net/71986B53DE5788BB";
    char s[] = {'h','t','t','p','s',':','/','/','w','.','e','y','d','a','t','a','.','n','e','t','/','F','6','F','7','6','7','A','3','E','B','F','1','5','6','0','0','\0'};
    string results = post(s,"Ver=" + version + "&Mac=" + device_id + "&SingleCode=" + card,true);
    return results;
}

string UserLogin(string card, string password, string device_id, string version) {
    string result = post("https://w.eydata.net/71986B53DE5788BB", "UserName=" + card + "&UserPwd=" + password + "&Version=" + version + "&Mac=" + device_id+ "&MacTwo=" + device_id,true);
    return result;
}


string getData(string url,const string& StatusCode, const string& card) {
    string url1 = "https://w.eydata.net/d968852c5e4b9c84";
    string result = post(url, "StatusCode=" + StatusCode + "&UserName=" + card,true);
    print("getData:%s",result.c_str());
    return result;
}

std::vector<string> split(string &text)
{
    std::regex ws_re("\\s+");  // white space
    return vector<string>(sregex_token_iterator(text.begin(), text.end(), ws_re, -1), sregex_token_iterator());
}

//void testSplit(){
//    string str ;
//    vector<std::string> results = split(str);
//    for (const string& s : results)
//        std::cout << s << std::endl;
//}

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

