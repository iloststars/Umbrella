#include <unistd.h>
#include <cstdio>
#include <string>
#include <sys/syscall.h>   	// 读取数据
#include <sys/uio.h>	   	// 读取数据
#include <dirent.h>
#include <loginUtils.h>
#include <thread>

using namespace std;

typedef struct{
    int x;
    int y;
}Soldier;

typedef struct {
    int id;
    int mapX;
    int mapY;
    float hp;
    float rayX;
    float rayY;
    float h;
    float distance;
    int cdd;
    int cdz;
    int cdzid;
    int hc;
} Hero;

typedef struct{
    Hero players[5];
    int buff[12];
    Soldier soldier[256];
    int num;
    int status;
}Data;

int find_pid_of(const char *process_name)	// 获取PID函数
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
    if (dir == NULL)
    {
        return -1;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        id = atoi(entry->d_name);
        if (id != 0)
        {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp)
            {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(process_name, cmdline) == 0)
                {
                    pid = id;
                    break;
                }
            }
        }
    }
    closedir(dir);
    return pid;
}

void killScript(char *path)
{
    int id = -1;
    DIR *dir;
    FILE *fp;
    struct dirent *entry;
    char cmdline[256], filename[32];
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

int readStructure(int pid, long address, Data* _data) 	// 读取核心函数
{
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = _data;
    local[0].iov_len = sizeof(Data);
    remote[0].iov_base = (void *)address;
    remote[0].iov_len = sizeof(Data);
    return (int)syscall(SYS_process_vm_readv, pid, local, 1, remote, 1, 0);
}

int writeStructure(int pid,long address,Data* _data){
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = _data;
    local[0].iov_len = sizeof(Data);
    remote[0].iov_base = (void *) address;
    remote[0].iov_len = sizeof(Data);
    return (int)syscall(__NR_process_vm_writev, pid, local, 1, remote, 1, 0);
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
            print("游戏或软件退出");
            exit(0);
        }
        usleep(5000 * 1000);
    }
}

int pidApp;
int pidGame;
long addr_src;
long addr_tar;
Data data;

int main(int argc, char **argv){
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
    pidGame= find_pid_of("com.tencent.tmgp.sgame");
    addr_src = atol(getCmdResult("su -c 'cat /sdcard/Android/data/com.tencent.tmgp.sgame/files/.conf'").c_str());
    addr_tar = atol(address.c_str());
//    char cmd[128];
//    printf("addr_src %ld",addr_src);
//    printf("addr_tar %ld",addr_tar);
//    sprintf(cmd,"su -c echo %d %d %ld %ld> /data/local/tmp/123",pidGame,pidApp,addr_src,addr_tar);
//    system(cmd);



    while (true){
        readStructure(pidGame,addr_src,&data);
//        printf("hp %f",data.players[0].hp);
        usleep(1000*10);
        writeStructure(pidApp,addr_tar,&data);
//        sprintf(cmd,"su -c echo %d %d > /data/local/tmp/456",data.status,data.num);
//        system(cmd);
    }

}