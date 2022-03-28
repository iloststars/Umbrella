#include <string>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
#include <dirent.h>
#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <pthread.h>
#include <cctype>
#include <cmath>
#include <jni.h>
#include <iosfwd>
#include <thread>
#include "loginUtils.h"
//#include "Memory.h"
#include "jniutils.h"
#include <android/looper.h>
#include "injectMemory.h"

#define BYTE0 0x0000000000
#define BYTE4 0x0000000004
#define BYTE8 0x0000000008
#define BYTE16 0x0000000010
#define BYTE128 0x000000080
#define BYTE256 0x0000000100
#define BYTE1024 0x000000400

//JNIEnv* env = nullptr;

typedef struct {
    int x;
    int y;
} Soldier;

typedef struct {
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
} Hero;

typedef struct {
    Hero players[5];
    int buff[12];
    Soldier soldier[256];
    int num;
    int status;
} Data;

Data data;


typedef struct {
    int X;
    int Y;
    int w;
    int H;
} Rect; //value(float)(数据）

Rect rect;

typedef struct {
    long mainadr;
    long CantseeXY_relay_adr;
    long CantseeXYaddr;
    long FieldofvisionXYaddr;
    long Isitinviewaddress;
    long HPaddr;
//    long XYaddr;
    long HCaddr;
    long zhsjn2adr;
    long zhsjn5adr;
    long zhsID5adr;
    long BUFFADR;
} Longaddr; //address(地址）

typedef struct {
    int Dwordvalue;
    float Floatvalue;
    long Pointeraddress;
    //|||||||||||||||||||||||||||||||||||||||||
    char buffstring[BYTE128] = "\0";
    char Datastring[BYTE1024 * 2] = "\0";
    float MatrixMem[BYTE16] = {0.0f};
    //|||||||||||||||||||||||||||||||||||||||||
    int size_w = 0;             //分辨率宽
    int px = 0;                  //分辨率高/2
    int py = 0;                  //分辨率宽/2
    pid_t processPID = -1;      //PID
    int SDPDid = 0;             //判断阵营
    double SDahm = 0.0;        //算法
    int Theenemynumber = 0;  //敌方人数
    long Global_state = 0x0;    //全局状态
    long matrixadr = 0x0;       //矩阵首地址
    long Thesoldiers = 0x0;     //兵线初始化地址
    long meXaddr = 0x0;        //自己坐标X地址
    int XXDATa = 0;            // |
    int YYDATa = 0;           // |}
    long BUFFtimingaddr[12] = {0};
} Constant_pool;//(常量池)/+/+/+/+/+/+/+

//jobject getContext(){
//    jclass activityThreadClz = env->FindClass("android/app/ActivityThread");
//    jmethodID currentActivityThreadMtd = env->GetStaticMethodID(activityThreadClz, "currentActivityThread", "()Landroid/app/ActivityThread;");
//    jobject activityThread = env->CallStaticObjectMethod(activityThreadClz, currentActivityThreadMtd);
//    jmethodID getApplicationMtd = env->GetMethodID(activityThreadClz, "getApplication", "()Landroid/app/Application;");
//    return env->CallObjectMethod(activityThread, getApplicationMtd);
//}

int tmp_num;

bool SoldierswireID_IF(int ID) {
    if ((ID >= 6000 && ID <= 6005) || ID == 6014 || ID == 6015 || ID == 6100 || ID == 6103 ||
        ID == 6114 || ID == 6115 || ID == 60000 || (ID >= 60011 && ID <= 60015) || ID == 60021 ||
        ID == 60030 || ID == 60031 || (ID >= 60041 && ID <= 60044) || ID == 60051) {
        return true;
    } else {
        return false;
    }
}

//using Pointer = int;
using namespace std;            //排序定义
Constant_pool Constants;         //创建常量结构体
Longaddr Globaladdr[BYTE16];     //(main, 血量, 大招CD, 回程判断).addr

extern "C"
JNIEXPORT jint

JNICALL
Java_com_tencent_special_httpdns_base_Jni_getNetworkStack(JNIEnv *env, jclass clazz)
{
    return 0;
}


extern "C"
JNIEXPORT jint
JNICALL
Java_com_tencent_special_httpdns_base_Jni_getNetworkStack_0(JNIEnv *env, jclass clazz){
    return 0;
}

extern "C"
JNIEXPORT jdouble

JNICALL
Java_com_tencent_tencentmap_ldssdk_service_e_b(JNIEnv *env, jclass clazz, jint i, jint i2,
                                               jdoubleArray d_arr) {
    // TODO: implement b()
}

extern "C"
JNIEXPORT jint

JNICALL
Java_com_tencent_tencentmap_ldssdk_service_e_o(JNIEnv *env, jclass clazz, jbyteArray b_arr,
                                               jint i) {
    // TODO: implement o()Pointeraddress
}

extern "C"
JNIEXPORT jdouble

JNICALL
Java_com_tencent_tencentmap_ldssdk_service_e_r(JNIEnv *env, jclass clazz, jdouble d, jint i,
                                               jint i2) {
    // TODO: implement r()
}

extern "C"
JNIEXPORT jint

JNICALL
Java_com_tencent_tencentmap_ldssdk_service_e_v(JNIEnv *env, jclass clazz, jstring str) {
    // TODO: implement v()
}

extern "C"
JNIEXPORT jint

JNICALL
Java_com_tencent_tencentmap_ldssdk_service_e_w(JNIEnv *env, jclass clazz, jstring str,
                                               jstring str2) {
    // TODO: implement w()
}


InjectMemory memory;

float getdistance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}

//void getResolvingPower() {
//    long resolutionratio = memory.getModuleBase("libunity.so:bss") + 0x19FF2C;
//    Constants.size_w = memory.readDword(resolutionratio + 0x4);
//    Constants.px = memory.readDword(resolutionratio);
//    Constants.py = Constants.size_w;
//    //LOGE("高 : %d 宽 : %d \n", Constants.px, Constants.py);
//}

void getResolvingPower() {
//    std::string result = getCmdResult("su -c dumpsys display | grep mOverrideDisplayInfo");
//    int pos = result.find("real");
//    int x = 2340, y = 1080;
//    if (pos != result.npos) {
//        result = result.substr(pos + 5, 11);
//        sscanf(result.c_str(), "%d x %d", &y, &x);
//    } else {
//        printf("Cannot get screen size,set to default!\n");
//        std::string result1 = getCmdResult("wm size");
//        sscanf(result1.c_str(), "Physical size: %ix%i", &y, &x);
//    }
//    Constants.size_w = (x > y) ? y : x;
//    Constants.px = (x > y) ? x : y;
//    Constants.py = (x < y) ? x : y;
//    printf("ScreenSize : %i x %i\n", ((x > y) ? x : y), ((x < y) ? x : y));
}


Rect CalMatrixMem(int X, int Y) {
    static Rect rect{};
    double xm1 = X * 0.001;
    double zm1 = Y * 0.001;
    double radio = zm1 * Constants.MatrixMem[11] + Constants.MatrixMem[15];
    if (radio < 0) {
        radio = -radio;
    }
    if (radio > 0.01) {
        rect.X = (int) ((Constants.px / 2.0) +
                        (xm1 * Constants.MatrixMem[0] + Constants.MatrixMem[12]) *
                        (Constants.px / 2.0) / radio);
        rect.Y = (int) ((Constants.py / 2.0) -
                        (zm1 * Constants.MatrixMem[9] + Constants.MatrixMem[13]) *
                        (Constants.py / 2.0) / radio);
        int w = (int) ((Constants.py / 2.0) -
                       (xm1 * Constants.MatrixMem[1] + 4.0 * Constants.MatrixMem[5] +
                        zm1 * Constants.MatrixMem[9] +
                        Constants.MatrixMem[13]) * (Constants.py / 2.0) / radio);
        rect.H = (int) ((rect.Y - w) / 2.0);
    }
    return rect;
}

void CYCLE_Data() {
//    char stringtmp[BYTE256]; //中转存
    float Me_value_X = 0, Me_value_Y = 0; //矩阵自己XY_value
    while ((int) memory.readDword(Constants.Global_state) == 1) {
        for (int i = 0; i < 16; ++i) {
            Constants.MatrixMem[i] = memory.readFloat(Constants.matrixadr + i * 4);
        }

        for (int b = 0; b < 12; b++) {
            data.buff[b] = memory.readDword(Constants.BUFFtimingaddr[b]) / 1000;
            //print("BUFFCD[%d] = %d\n", b, PlayerData[b].BUFFCD);
        }
        Me_value_X = Constants.MatrixMem[12] / Constants.MatrixMem[0] * -1.0f * 1000.0f;
        Me_value_Y = Constants.MatrixMem[13] / Constants.MatrixMem[9] * -1.0f * 1000.0f;
        for (int i = 0; i < Constants.Theenemynumber; i++) {
            if (memory.readDword(Globaladdr[i].Isitinviewaddress) != 16777216 &&
                memory.readDword(Globaladdr[i].Isitinviewaddress) > 3) {
                //有视野
                Constants.XXDATa = (int) (memory.readFloat(Globaladdr[i].FieldofvisionXYaddr + 0x0) * 1000);
                Constants.YYDATa = (int) (memory.readFloat(Globaladdr[i].FieldofvisionXYaddr + 0x8) * 1000);
            } else {
                //无视野
                long local_node_addr = memory.readPointer(Globaladdr[i].CantseeXY_relay_adr) + 0x10;
                long local_X_addr = memory.readPointer(local_node_addr) + (memory.readDword(local_node_addr + 0x18) << 4);
                Constants.XXDATa = memory.readDword(local_X_addr + 0x0);
                Constants.YYDATa = memory.readDword(local_X_addr + 0x8);
            }
            rect = CalMatrixMem(Constants.XXDATa, Constants.YYDATa);
            data.players[i].mapX = (int) (Constants.XXDATa * Constants.SDahm);
            data.players[i].mapY = (int) (Constants.YYDATa * -1 * Constants.SDahm);
            data.players[i].hp = memory.readDword(Globaladdr[i].HPaddr) * 1.0f /
                                 memory.readDword(Globaladdr[i].HPaddr + 0x8);
            data.players[i].rayX = rect.X;
            data.players[i].rayY = rect.Y;
            data.players[i].h = rect.H;
            data.players[i].distance = getdistance(Me_value_X, Me_value_Y, Constants.XXDATa, Constants.YYDATa); //距离
            data.players[i].cdd = memory.readDword(Globaladdr[i].zhsjn2adr) / 8192000;
            data.players[i].cdz = memory.readDword(Globaladdr[i].zhsjn5adr) / 8192000;
            data.players[i].cdzid = memory.readDword(Globaladdr[i].zhsID5adr) - 80100;
            data.players[i].hc = memory.readDword(Globaladdr[i].HCaddr);
            //LOGE("%d", data.players[i].cdzid);
        }//敌人人物for

        //兵线
        tmp_num = 0;
        for (int i = 0; i < memory.readDword(Constants.Thesoldiers + 0x14); i++) {
            long Thesoldiersmain = memory.readPointers(Constants.Thesoldiers, {(0x10 * i), 0});
            if ((int) memory.readDword(Thesoldiersmain + 0x2C) == Constants.SDPDid &&
                SoldierswireID_IF((int) memory.readDword(Thesoldiersmain + 0x20))) {
                if ((int) memory.readDword(memory.readPointer(Thesoldiersmain + 0xD8) + 0x78) >
                    0) {
                    long Xaddr = memory.readPointers(Thesoldiersmain + 0x220,{0xB0, 0, 0x10, 0});
                    int smapX = (int) (memory.readDword(Xaddr + 0x0) * Constants.SDahm);
                    int smapY = (int) (memory.readDword(Xaddr + 0x8) * -1 * Constants.SDahm);
                    if (smapX != 0 && smapY != 0) {
                        data.soldier[tmp_num].x = smapX;
                        data.soldier[tmp_num].y = smapY;
                        tmp_num++;
//                        LOGE("兵线 = %d - %d", data.soldier[tmp_num].x,data.soldier[tmp_num].y);

                    }
                }
            }
        }
        data.num = tmp_num;
        data.status = 1;
        usleep(20 * 1000);
    } //总(while)
}

void LOAD_DATAaddr() {
    for (int i = 0; i < Constants.Theenemynumber; i++) {
        data.players[i].id = memory.readDword(Globaladdr[i].mainadr + 0x20);//英雄代表id
        Globaladdr[i].HPaddr = memory.readPointer(Globaladdr[i].mainadr +  0xD8) + 0x78; //血量地址
        Globaladdr[i].CantseeXY_relay_adr = memory.readPointers(Globaladdr[i].mainadr + 0x220, {0xB0, 0});  //坐标中继地址
        Globaladdr[i].HCaddr = memory.readPointers(Globaladdr[i].mainadr + 0xC8,{(0x98 + 6 * 0x10), 0xC8, 0x20});//回城判断地址
        Globaladdr[i].CantseeXYaddr = memory.readPointers(Globaladdr[i].mainadr + 0x148,{0x10, 0}); //坐标地址
        Globaladdr[i].zhsjn2adr = memory.readPointers(Globaladdr[i].mainadr + 0xC8,{(0x98 + 2 * 0x10), 0x78, 0x30}); //英雄大招cd地址
        Globaladdr[i].zhsjn5adr = memory.readPointers(Globaladdr[i].mainadr + 0xC8,{(0x98 + 5 * 0x10), 0x78, 0x30}); //召唤师闪现类型cd地址
        Globaladdr[i].zhsID5adr =
                Globaladdr[i].zhsjn5adr + 0x310;                       //召唤师闪现类ID地址
    }
}

//void Load_buffaddr(int PDid) {
//    int order[2][12] = {{6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4,  5},
//                        {0, 1, 2, 3, 4,  5,  6, 7, 8, 9, 10, 11}};
//    long libGameCore_startadr = memory.getModuleBase("libGameCore.so:bss");
////    long address = memory.readPointers(libGameCore_startadr + 0x1B28, {0x358, 0x68, 0xC8, 0});
//    long address = memory.readPointers(libGameCore_startadr + 0x24b8,
//                                       {0x38, 0x18, 0x30, 0xA8, 0xC8, 0});
//
//    for (int k = 0; k < 12; k++) {
//        Constants.BUFFtimingaddr[k] =
//                memory.readPointer(address + order[PDid - 1][k] * 0x10) + 0x1C8;
//    }
//}

void Load_buffaddr(const int PDid)
{
    const int order[2][12] = { {6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} };
    long libGameCore_startadr = memory.getModuleBase("libGameCore.so:bss");
    long address = memory.readPointers(libGameCore_startadr + 0x24b8, {0xE8, 0xE0, 0xE8, 0x68, 0xC8});
    for (int k = 0; k < memory.readDword(address + 0x14); k++)
    {
        long buffMain_addr_cd = memory.readPointers(address, {(k * 0x10), 0});
        int buff_id = memory.readPointer(buffMain_addr_cd + 0x98);
        int buff_x = memory.readDword(buffMain_addr_cd + 0x220 + 0);
        int buff_y = memory.readDword(buffMain_addr_cd + 0x220 + 8);
        if ((buff_id == 6010 || buff_id == 6310) && buff_x < 0 && buff_y > 0) {
            Constants.BUFFtimingaddr[ order[PDid][0] ] = buffMain_addr_cd + 0x1C8;
            continue;
        }
        if ((buff_id == 6008 || buff_id == 60081 || buff_id == 6306 || buff_id == 63061) && buff_x < 0 && buff_y < 0) {
            Constants.BUFFtimingaddr[ order[PDid][1] ] = buffMain_addr_cd + 0x1C8;
            continue;
        }
        if ((buff_id == 6013 || buff_id == 60131 || buff_id == 6313 || buff_id == 63131) && buff_x < 0 && buff_y > 0) {
            Constants.BUFFtimingaddr[ order[PDid][2] ] = buffMain_addr_cd + 0x1C8;
            continue;
        }
        if ((buff_id == 6011 || buff_id == 6311) && buff_x > 0 && buff_y < 0) {
            Constants.BUFFtimingaddr[ order[PDid][3] ] = buffMain_addr_cd + 0x1C8;
            continue;
        }
        if ((buff_id == 6006 || buff_id == 60061 || buff_id == 60062 || buff_id == 6308 || buff_id == 63081 || buff_id == 63082) && buff_x < 0 && buff_y < 0) {
            Constants.BUFFtimingaddr[ order[PDid][4] ] = buffMain_addr_cd + 0x1C8;
            continue;
        }
        if ((buff_id == 6007 || buff_id == 6307) && buff_x > 0 && buff_y < 0) {
            Constants.BUFFtimingaddr[ order[PDid][5] ] = buffMain_addr_cd + 0x1C8;
            continue;
        }
        if ((buff_id == 6010 || buff_id == 6310) && buff_x > 0 && buff_y < 0) {
            Constants.BUFFtimingaddr[ order[PDid][6] ] = buffMain_addr_cd + 0x1C8;
            continue;
        }
        if ((buff_id == 6008 || buff_id == 60081 || buff_id == 6306 || buff_id == 63061) && buff_x > 0 && buff_y > 0) {
            Constants.BUFFtimingaddr[ order[PDid][7] ] = buffMain_addr_cd + 0x1C8;
            continue;
        }
        if ((buff_id == 60132 || buff_id == 60133 || buff_id == 63132 || buff_id == 63133) && buff_x > 0 && buff_y < 0) {
            Constants.BUFFtimingaddr[ order[PDid][8] ] = buffMain_addr_cd + 0x1C8;
            continue;
        }
        if ((buff_id == 6011 || buff_id == 6311) && buff_x < 0 && buff_y > 0) {
            Constants.BUFFtimingaddr[ order[PDid][9] ] = buffMain_addr_cd + 0x1C8;
            continue;
        }
        if ((buff_id == 6006 || buff_id == 60061 || buff_id == 60062 || buff_id == 6308 || buff_id == 63081 || buff_id == 63082) && buff_x > 0 && buff_y > 0) {
            Constants.BUFFtimingaddr[ order[PDid][10] ] = buffMain_addr_cd + 0x1C8;
            continue;
        }
        if ((buff_id == 60071 || buff_id == 63071) && buff_x < 0 && buff_y > 0) {
            Constants.BUFFtimingaddr[ order[PDid][11] ] = buffMain_addr_cd + 0x1C8;
            continue;
        }
    }
}

void Load_Thesoldiers() {
    long libGameCore_startadr = memory.getModuleBase("libGameCore.so:bss");
    Constants.Thesoldiers = memory.readPointers(libGameCore_startadr + 0x24b8,
                                                {0x38, 0x18, 0x30, 0xC0});
    //printf("Var : %i addr : %lX\n", readDword(Constants.Thesoldiers), Constants.Thesoldiers);
}

//void Load_meXaddr() {
//    long libGameCore_startadr = memory.getModuleBase("libGameCore.so:bss");
//    Constants.meXaddr = memory.readPointers(libGameCore_startadr + 0x24b8,
//                                            {0x38, 0x98, 0x98, 0xA8, 0x160, 0});
//    //printf("Var : %i addr : %lX\n", readDword(Constants.meXaddr), Constants.meXaddr);
//}


void Load_FieldofvisionXYaddr() {
    long libil2cpp_startadr = memory.getModuleBase("libil2cpp.so:bss");
    long address = memory.readPointers(libil2cpp_startadr + 0xCB45D0, {0x100, 0x10, 0x48, 0x40});
    int truesl = memory.readDword(address + 0x8);
    for (int i = 0; i < truesl; i++) {
        long Mainaddr2 = memory.readPointers(address, {(i * 8 + 0x20), 0xC0, 0});
        for (int H = 0; H < Constants.Theenemynumber; H++) {
            if (memory.readDword(Globaladdr[H].mainadr + 0xC) == memory.readDword(Mainaddr2 + 0x110))
            {
                Globaladdr[H].Isitinviewaddress = Mainaddr2 + 0x15C;
                Globaladdr[H].FieldofvisionXYaddr = Mainaddr2 + 0x12C;
                break;
            }
        }
    }
}


void Load_matrixadr(int *PDid) {
    int fx = 0;
    long libil2cpp_startadr = memory.getModuleBase("libil2cpp.so:bss");
    Constants.matrixadr = memory.readPointers(libil2cpp_startadr + 0xC8FF10,
                                              {0xA0, 0, 0x10, 0xC0});
    if (memory.readDword(Constants.matrixadr) > 0) { *PDid = 2, fx = 1; }
    else { *PDid = 1, fx = -1; }   //判断阵营
    Constants.SDahm = (Constants.size_w * 0.314814815 / 2) / 50000 * fx;                 //坐标算法
    Constants.SDPDid = *PDid;
}

//void Load_mainaddr(int PDid) {
//    Constants.Theenemynumber = 0;
//    long libGameCore_startadr = memory.getModuleBase("libGameCore.so:bss");
//    long address = memory.readPointer(libGameCore_startadr + 0x24b8)+0x8;
//    for (int i = 0; i < memory.readDword(address + 0x14)-3; i++) {
//        long Mainaddress = memory.readPointers(address, {(i * 0x10 + 0x30), 0x10, 0x18, 0});
//        if (memory.readDword(Mainaddress + 0x2C) == PDid ) {
//            long X_address = memory.readPointers(Mainaddress + 0x148, {0x10, 0});
//            if (memory.readDword(X_address) != 0 || memory.readDword(X_address + 0x8) != 0) {
//                Globaladdr[Constants.Theenemynumber].mainadr = Mainaddress;
//                //printf("Mainaddr[%i] = %lX\n", Constants.Theenemynumber, Globaladdr[Constants.Theenemynumber].mainadr);
//                Constants.Theenemynumber++;
//            }
//        }
//    }
//}

void Load_mainaddr(int PDid)
{
    Constants.Theenemynumber = 0;
    long libGameCore_startadr = memory.getModuleBase( "libGameCore.so:bss");
    long address = memory.readPointers(libGameCore_startadr + 0x24B8, {0x8});
    for (int i = 0; i < memory.readDword(address + 0x14) - 3; i++)
    {
        long Mainaddress = memory.readPointers(address, {(i * 0x10 + 0x30), 0x10, 0x18, 0});
        if (memory.readDword(Mainaddress + 0x2C) == PDid)
        {
            long local_node_addr = memory.readPointer(memory.readPointer(memory.readPointer(Mainaddress + 0x220) + 0xE8) + 0x28) + 0x0;
            long local_X_addr = memory.readPointer(local_node_addr + 0x40) + (memory.readDword(local_node_addr + 0x3C) << 4);
            if (memory.readDword(local_X_addr) != 0 || memory.readDword(local_X_addr + 0x8) != 0) {
                Globaladdr[Constants.Theenemynumber].mainadr = Mainaddress;
                //printf("Mainaddr[%i] = %lX\n", Constants.Theenemynumber, Globaladdr[Constants.Theenemynumber].mainadr);
                Constants.Theenemynumber++;
            }
        }
    }
}


void Camp() {
    int PDid = 0;
    Load_matrixadr(&PDid);        //获取距阵首地址
    Load_mainaddr(PDid);          //获取所有敌人main地址
    Load_FieldofvisionXYaddr();    //获取有视野坐标
//    Load_meXaddr();               //获取自己坐标X地址
    Load_Thesoldiers();           //获取兵线初始地址
    Load_buffaddr(PDid-1);        //获取12个BUFF地址
    LOAD_DATAaddr();             //获取所有敌人(英雄代表id, 血量地址, 回城判断地址, 大招cd地址)
    CYCLE_Data();
    data.status = 0;
}

//易游
string verify(const string &singleCode, const string &version, const string &mac) {
    string status = SingleCodeLogin(singleCode, mac, version);
    print("%s", status.c_str());
    if (status.length() == 32) {
        return status;
    } else {
        sleep(600000);
    }
    return "12654186457";
}

void startDraw() {
    LOGE("%ld", &data);
    char address[32];
    sprintf(address, "%ld", &data);
    FILE *fp = fopen("/sdcard/Android/data/com.tencent.tmgp.sgame/files/.conf", "w");
    if (fp != nullptr) {
        fprintf(fp, "%s", address);
        fclose(fp);
    } else {
    }

    char singleCode[32];
    char version[32];
    char mac[32];
    char size_w[32],px[32],py[32];
    fp = fopen("/sdcard/Android/data/com.tencent.tmgp.sgame/files/.key", "r");
    if (fp != nullptr) {
        fscanf(fp, "%s %s %s %s %s %s", singleCode, version, mac,size_w,px,py);
        fclose(fp);
    }
    std::thread(verify, singleCode, version, mac).join();
    LOGE("%s %s %s", singleCode, version, mac);
    memory.init("com.tencent.tmgp.sgame");
    getResolvingPower();   //获取_分辨率
    Constants.size_w = atoi(size_w);
    Constants.px = atoi(px);
    Constants.py = atoi(py);
    sleep(20);
    Constants.Global_state = memory.getModuleBase("libGameCore.so:bss") + 0x452c08;
    for (;;) {
        int state = (int) memory.readDword(Constants.Global_state);
        LOGE("state = %d", state);
        if (state == 1) {
            usleep(1000 * 1000);
            Camp();
        }
        usleep(40 * 1000);
    }

}

JNIEXPORT jint
JNI_OnLoad(JavaVM
           *vm,
           void *reserved
) {
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK)
        return -1;
    std::thread th(startDraw);
    th.detach();
    return
            JNI_VERSION_1_4;
}