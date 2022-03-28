#include <stdio.h>		 	// 基本头文件
#include <dirent.h>			// 获取PID需要的
#include <stdlib.h>			// 获取PID需要的(atoi)
#include <unistd.h>			// 读取数据
#include <sys/uio.h>	   	// 读取数据
#include <algorithm>           // 排序
#include <sys/syscall.h>   	// 读取数据
#include <math.h>          //取绝对值(float)
#include <initializer_list>   //多级跳需要
#include <string>         //切换工作目录需要
#include <thread>
#include <vector>
//#include <bsphp.h>
#include <loginUtils.h>
#define BYTE0 0x0000000000
#define BYTE4 0x0000000004
#define BYTE8 0x0000000008
#define BYTE16 0x0000000010
#define BYTE128 0x000000080
#define BYTE256 0x0000000100
#define BYTE1024 0x000000400

using Pointer = long;

typedef struct {
    Pointer start;
    Pointer end;
} MemoryRange;

vector<MemoryRange> unsafeRanges;

int kk = 0;
int pidApp;
long addr_tar;
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
    int YYDATa = 0;            // |}
    long BUFFtimingaddr[12] = {0};
} Constant_pool;//(常量池)/+/+/+/+/+/+/+


using namespace std;            //排序定义
Constant_pool Constants;         //创建常量结构体
Longaddr Globaladdr[BYTE16];     //(main, 血量, 大招CD, 回程判断).addr
Hero PlayerData[BYTE16];     //存储数据所有人(计算后)
struct iovec iov_ReadBuffer, iov_ReadOffset;//读函数定义

int find_pid_of(const char *process_name)    // 获取PID函数
{
    int id;
    pid_t pid = -1;
    DIR *dir;
    FILE *fp;
    char filename[32];
    char cmdline[256];
    struct dirent *entry;
    if (process_name == NULL)
        return -1;
    dir = opendir("/proc");
    if (dir == NULL) {
        return -1;
    }
    while ((entry = readdir(dir)) != NULL) {
        id = atoi(entry->d_name);
        if (id != 0) {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp) {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(process_name, cmdline) == 0) {
                    pid = id;
                    break;
                }
            }
        }
    }
    closedir(dir);
    return pid;
}

long getModuleBase(pid_t PID, char *module_name) {
    char *phgsr;
    char jjjj_N[64];
    long startaddr = 0;
    char path[256], line[1024];
    bool bssOF = false, LastIsSo = false;
    strcpy(jjjj_N, module_name);
    phgsr = strtok(jjjj_N, ":");
    module_name = phgsr;
    phgsr = strtok(NULL, ":");
    if (phgsr) {
        if (strcmp(phgsr, "bss") == 0) { bssOF = true; }
    }
    sprintf(path, "/proc/%d/maps", PID);
    FILE *p = fopen(path, "r");
    if (p) {
        while (fgets(line, sizeof(line), p)) {
            if (LastIsSo) {
                if (strstr(line, "[anon:.bss]") != NULL) {
                    sscanf(line, "%lx-%*lx", &startaddr);
                    break;
                } else {
                    LastIsSo = false;
                }
            }
            if (strstr(line, module_name) != NULL) {
                if (!bssOF) {
                    sscanf(line, "%lx-%*lx", &startaddr);
                    break;
                } else {
                    LastIsSo = true;
                }
            }
        }
        fclose(p);
    }
    return startaddr;
}

void initAntiReadMemoryRanges() {
    print("initAntiReadMemoryRanges");
    unsafeRanges.clear();
    Pointer start = 0, end = 0;
    char buffer[1024];
    char filename[32];
    if (Constants.processPID < 0) {
        snprintf(filename, sizeof(filename), "/proc/self/maps");
    } else {
        snprintf(filename, sizeof(filename), "/proc/%d/maps", Constants.processPID);
    }
    FILE *fp = fopen(filename, "r");
    if (fp != nullptr) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            sscanf(buffer, "%llx-%llx %*s", &start, &end);
            if (end - start < 0x5000) {
                unsafeRanges.push_back({start, end});
                print("do not Read: [0x%x, 0x%x]", start, end);
            }else{
                // print("can Read: [0x%x, 0x%x]", start, end);
            }
        }
        fclose(fp);
    }
}

int binary_search_lower_bound(Pointer target) {
    int low = 0, high = (int)unsafeRanges.size();
    int mid;

    while(low < high) {
        mid = low + ((high - low) >> 1);
        MemoryRange val = unsafeRanges[mid];
//            ALOGD("bin: %d{%d}%d:[0x%x, 0x%x]\n", low, mid, high, val.start, val.end);
        if (target <= val.start)//array[mid])
            high = mid;
        else if (target > val.end)
            low = mid + 1;
        else
            return mid;
    }
    return low;
}

bool canRead(Pointer address) {
    int ub = binary_search_lower_bound(address);
    if (unsafeRanges[ub].start <= address && address <= unsafeRanges[ub].end) {
        print("dangerous!: 0x%x in [0x%x, 0x%x]\n", address, unsafeRanges[ub].start, unsafeRanges[ub].end);
        return false;
    }
//        ALOGD("can Read: 0x%x in [0x%x, 0x%x]\n", address, unsafeRanges[ub].start, unsafeRanges[ub].end);
    return true;
}

void killScript(char *path) {
    int id = -1;
    DIR *dir;
    FILE *fp;
    struct dirent *entry;
    char cmdline[BYTE256], filename[BYTE16 * 2];
    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL) {
        id = atoi(entry->d_name);
        if (id != 0) {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp) {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(path, cmdline) == 0 && getpid() != id) {
                    char aa[64] = {0};
                    sprintf(aa, "kill -9 %d", id);
                    system(aa);
                }
            }
        }
    }
    closedir(dir);
}

void writeDataBin(const void *p, const char path[]) {
    FILE *fp = fopen(path, "wb");
    fwrite(p, sizeof(Data), 1, fp); // 写N条信息全部写入文件
    fclose(fp);
}

int writeStructure(int pid, long address, Data *_data) {
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = _data;
    local[0].iov_len = sizeof(Data);
    remote[0].iov_base = (void *) address;
    remote[0].iov_len = sizeof(Data);
    return (int) syscall(__NR_process_vm_writev, pid, local, 1, remote, 1, 0);
}

void readBuffer(long address, void *buffer, int size)    // 读取核心函数
{
    iov_ReadBuffer.iov_base = buffer;
    iov_ReadBuffer.iov_len = size;
    iov_ReadOffset.iov_base = (void *) address;
    iov_ReadOffset.iov_len = size;
    syscall(SYS_process_vm_readv, Constants.processPID, &iov_ReadBuffer, 1, &iov_ReadOffset, 1, 0);
}

int readDword(long address) // 传输地址(读)一个 Dword类型
{
    Constants.Dwordvalue = 0;
    readBuffer(address, &Constants.Dwordvalue, BYTE4);
    return Constants.Dwordvalue;
}

float readFloat(long address)  // 传输地址(读)一个 Float类型
{
    Constants.Floatvalue = 0.0;
    readBuffer(address, &Constants.Floatvalue, BYTE4);
    return Constants.Floatvalue;
}

long readPointer(long address) //传输地址(读) 跳指针(64位)
{
    Constants.Pointeraddress = 0x0;
    readBuffer(address, &Constants.Pointeraddress, BYTE8);
    return Constants.Pointeraddress & 0xFFFFFFFFFF;
}

long readPointers(long address, std::initializer_list<Pointer> offset) {
    for (Pointer Offset : offset) {
        address = readPointer(address) + Offset;
    }
    return address;
}


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


float getdistance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}

void changeWorkspace(const string &argv0) {//切换工作目录
    std::string path = argv0;
    int pos = path.find_last_of("/");
    path = path.substr(0, pos);
    chdir(path.c_str());
}

void toast(char out[]) {
    FILE *fp = fopen("toast.xml", "w");
    if (fp != nullptr) {
        fprintf(fp, "%s", out);
        fclose(fp);
    }
}

void getResolvingPower() {
    std::string result = getCmdResult("dumpsys display | grep mOverrideDisplayInfo");
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
    Constants.size_w = (x > y) ? y : x;
    Constants.px = (x > y) ? x : y;
    Constants.py = (x < y) ? x  : y;
    printf("ScreenSize : %i x %i\n", ((x > y) ? x : y), ((x < y) ? x : y));
}

//Rect CalMatrixMem(int X1, int Y1, const float Matrix[]) {
//    static Rect rect;
//    double XM = X1 * 0.001;
//    double ZM = Y1 * 0.001;
//    double radio = fabs(ZM * Matrix[11] + Matrix[15]);
//    rect.X = 0.0f, rect.Y = 0.0f, rect.w = 0.0f, rect.H = 0.0f;
//    if (radio > 0.01) {
//        rect.X = Constants.px + (XM * Matrix[0] + Matrix[12]) * Constants.px  / radio;
//        rect.Y = Constants.py - (ZM * Matrix[9] + Matrix[13]) * Constants.py / radio;
//        rect.w = Constants.py - (XM * Matrix[1] + 4.0f * Matrix[5] + ZM * Matrix[9] + Matrix[13]) * Constants.py / radio;
//        rect.H = ((rect.Y - rect.w) / 2.0f);
//    }
//    return rect;
//}

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
    while ((int) readDword(Constants.Global_state) == 1) {
        for (int i = 0; i < 16; ++i) {
            Constants.MatrixMem[i] = readFloat(Constants.matrixadr + i * 4);
        }

        for (int b = 0; b < 12; b++) {
            data.buff[b] = readDword(Constants.BUFFtimingaddr[b]) / 1000;
            //print("BUFFCD[%d] = %d\n", b, PlayerData[b].BUFFCD);
        }
        print("num = %d", Constants.Theenemynumber);
        for (int i = 0; i < Constants.Theenemynumber; i++) {
            if (readDword(Globaladdr[i].Isitinviewaddress) != 16777216 &&
                readDword(Globaladdr[i].Isitinviewaddress) > 3) {
                Constants.XXDATa = (int) (readFloat(Globaladdr[i].FieldofvisionXYaddr + 0x0) * 1000);
                Constants.YYDATa = (int) (readFloat(Globaladdr[i].FieldofvisionXYaddr + 0x8) * 1000);
            } else {
                Constants.XXDATa = readDword(Globaladdr[i].CantseeXYaddr + 0x0);
                Constants.YYDATa = readDword(Globaladdr[i].CantseeXYaddr + 0x8);
            }
            rect = CalMatrixMem(Constants.XXDATa, Constants.YYDATa);
            data.players[i].mapX = (int) (Constants.XXDATa * Constants.SDahm);
            data.players[i].mapY = (int) (Constants.YYDATa * -1 * Constants.SDahm);
            data.players[i].hp = readDword(Globaladdr[i].HPaddr) * 1.0f /
                                 readDword(Globaladdr[i].HPaddr + 0x8);
            data.players[i].rayX = rect.X;
            data.players[i].rayY = rect.Y;
            data.players[i].h = rect.H;
            data.players[i].distance = getdistance(readDword(Constants.meXaddr),
                                                   readDword(Constants.meXaddr + 0x8),
                                                   Constants.XXDATa, Constants.YYDATa);
            data.players[i].cdd = readDword(Globaladdr[i].zhsjn2adr) / 8192000;
            data.players[i].cdz = readDword(Globaladdr[i].zhsjn5adr) / 8192000;
            data.players[i].cdzid = readDword(Globaladdr[i].zhsID5adr) - 80100;
            data.players[i].hc = readDword(Globaladdr[i].HCaddr);
            print("id = %d", data.players[i].id);
        }//敌人人物for

        //兵线
        tmp_num = 0;
        for (int i = 33; i < readDword(Constants.Thesoldiers + 0x14); i++) {
            long Thesoldiersmain = readPointers(Constants.Thesoldiers, {(0x10 * i), 0});
            if ((int) readDword(Thesoldiersmain + 0x2C) == Constants.SDPDid &&
                SoldierswireID_IF((int) readDword(Thesoldiersmain + 0x20))) {
                if ((int) readDword(readPointer(Thesoldiersmain + 0xD8) + 0x78) !=
                    0) {
                    long Xaddr = readPointers(Thesoldiersmain + 0x148, {0x10, 0});
                    int smapX = (int) (readDword(Xaddr + 0x0) * Constants.SDahm);
                    int smapY = (int) (readDword(Xaddr + 0x8) * -1 * Constants.SDahm);
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
        writeStructure(pidApp, addr_tar, &data);
//        writeDataBin(&data,"corecc");
        usleep(20 * 1000);
    } //总(while)
}

void LOAD_DATAaddr() {
    for (int i = 0; i < Constants.Theenemynumber; i++) {
        data.players[i].id = readDword(Globaladdr[i].mainadr + 0x20);                //英雄代表id
        Globaladdr[i].HCaddr = readPointers(Globaladdr[i].mainadr + 0xC8, {(0x98 + 6 * 0x10), 0xC8, 0x20}); //回城判断地址
        Globaladdr[i].HPaddr = readPointer(Globaladdr[i].mainadr + 0xD8) + 0x78;  //血量地址
        Globaladdr[i].CantseeXYaddr = readPointers(Globaladdr[i].mainadr + 0x148, {0x10, 0}); //坐标地址
        Globaladdr[i].zhsjn2adr = readPointers(Globaladdr[i].mainadr + 0xC8,
                                               {(0x98 + 2 * 0x10), 0xB8, 0x80}); //英雄大招cd地址
        Globaladdr[i].zhsjn5adr = readPointers(Globaladdr[i].mainadr + 0xC8,
                                               {(0x98 + 5 * 0x10), 0xB8,
                                                0x80}); //召唤师闪现类型cd地址
        Globaladdr[i].zhsID5adr =
                Globaladdr[i].zhsjn5adr + 0x310;                       //召唤师闪现类ID地址
    }
}

void Load_buffaddr(const int PDid)
{
    const int order[2][12] = { {6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} };
    long libGameCore_startadr = getModuleBase(Constants.processPID, "libGameCore.so:bss");
    long address = readPointers(libGameCore_startadr + 0x2080, {0xE8, 0xE0, 0xE8, 0x68, 0xC8});
    for (int k = 0; k < readDword(address + 0x14); k++)
    {
        long buffMain_addr_cd = readPointers(address, {(k * 0x10), 0});
        int buff_id = readDword(buffMain_addr_cd + 0x98);
        int buff_x = readDword(buffMain_addr_cd + 0x220 + 0);
        int buff_y = readDword(buffMain_addr_cd + 0x220 + 8);
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
    long libGameCore_startadr = getModuleBase(Constants.processPID, "libGameCore.so:bss");
    Constants.Thesoldiers = readPointers(libGameCore_startadr + 0x2080,
                                         {0x38, 0x18, 0x20, 0xC0});
    //printf("Var : %i addr : %lX\n", readDword(Constants.Thesoldiers), Constants.Thesoldiers);
}

void Load_meXaddr() {
    long libGameCore_startadr = getModuleBase(Constants.processPID, "libGameCore.so:bss");
    Constants.meXaddr = readPointers(libGameCore_startadr + 0x2080,
                                     {0x38, 0x98, 0x98, 0xA8, 0x160, 0});
    //printf("Var : %i addr : %lX\n", readDword(Constants.meXaddr), Constants.meXaddr);
}


void Load_FieldofvisionXYaddr() {
    long libil2cpp_startadr = getModuleBase(Constants.processPID, "libil2cpp.so:bss");
    long address = readPointers(libil2cpp_startadr + 0xC03B38, {0xA0, 0x60, 0x30, 0x10});
    int truesl = readDword(address + 0x8);
    for (int i = 0; i < truesl; i++) {
        long Mainaddr2 = readPointers(address, {(i * 8 + 0x20), 0xC0, 0});
        for (int H = 0; H < Constants.Theenemynumber; H++) {
            if (readDword(Globaladdr[H].mainadr - 0x5B4) == readDword(Mainaddr2 + 0x108)) {
                Globaladdr[H].Isitinviewaddress = Mainaddr2 + 0x154;
                Globaladdr[H].FieldofvisionXYaddr = Mainaddr2 + 0x124;
                break;
            }
        }
    }
}


void Load_matrixadr(int *PDid)
{
    int fx = 0;
    long libil2cpp_startadr = getModuleBase(Constants.processPID, "libil2cpp.so:bss");
    Constants.matrixadr = readPointers(libil2cpp_startadr + 0xC05FD8, {0xA0, 0, 0x10, 0xC0});
    if (readDword(Constants.matrixadr) > 0) {*PDid = 2, fx = 1;} else {*PDid = 1, fx = -1;}   //判断阵营
    Constants.SDahm = (Constants.size_w * 0.314814815 / 2) / 50000 * fx;                 //坐标算法
    Constants.SDPDid = *PDid;
}


void Load_mainaddr(int PDid)
{
    Constants.Theenemynumber = 0;
    long libGameCore_startadr = getModuleBase(Constants.processPID, "libGameCore.so:bss");
    long address = readPointer(libGameCore_startadr + 0x2080) + 0x8;
    for (int i = 0; i < readDword(address + 0x14) - 3; i++)
    {
        long Mainaddress = readPointers(address, {(i * 0x10 + 0x30), 0x10, 0x18, 0});
        if (readDword(Mainaddress + 0x2C) == PDid)
        {
            long X_address = readPointers(Mainaddress + 0x148, {0x10, 0});
            if (readDword(X_address) != 0 || readDword(X_address + 0x8) != 0) {
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
    Load_meXaddr();               //获取自己坐标X地址
    Load_Thesoldiers();           //获取兵线初始地址
    Load_buffaddr(PDid-1);        //获取12个BUFF地址
    LOAD_DATAaddr();             //获取所有敌人(英雄代表id, 血量地址, 回城判断地址, 大招cd地址)
    CYCLE_Data();
    data.status = 0;
    writeStructure(pidApp, addr_tar, &data);
}

//易游
string verify(const string &singleCode, const string &version) {
	string status = SingleCodeLogin(singleCode,getUUID(), version);
	print("%s", status.c_str());
	if (status.length() == 32) {
		return status;
	}else{
		sleep(600000);
	}
	return "12654186457";
}



//检查软件是否退出
void checkApp(const string &packageName) {
    pid_t appPid;
    pid_t gamePid;
    while (true) {
        appPid = find_pid_of(packageName.c_str());
        gamePid = find_pid_of("com.tencent.tmgp.sgame");
        if (appPid <= 0 || gamePid <= 0) {
            toast("游戏或软件退出");
            exit(0);
        }
        usleep(5000 * 1000);
    }
}

int main(int argc, char **argv) {
    killScript(argv[0]);
    string package_name = argv[1];
    string workspace = argv[2];
    string singleCode = argv[3];
    string version = argv[4];
    string address = argv[5];

    thread(checkApp, package_name).detach();
    thread(verify,singleCode,version).join();

    addr_tar = atol(address.c_str());
    pidApp = find_pid_of(package_name.c_str());

    Constants.processPID = find_pid_of("com.tencent.tmgp.sgame");
    if (Constants.processPID == -1) {
        print("还未启动游戏\n");
        exit(BYTE0);
    }
    //安全范围
    initAntiReadMemoryRanges();
    Constants.Global_state = getModuleBase(Constants.processPID, "libGameCore.so:bss") + 0x44F6F8;
    if (Constants.Global_state < 0xFFFFFFFFF) {
        print("数据获取错误\n");
        exit(BYTE0);
    }
    getResolvingPower();   //获取_分辨率

    for (;;) {
        if ((int) readDword(Constants.Global_state) == 1) {
            usleep(1000 * 1000);
            Camp();
        }
        usleep(40 * 1000);
    }
    return BYTE0;
}

