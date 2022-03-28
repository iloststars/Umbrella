#include <unistd.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <csignal>
#include <csetjmp>
#include <vector>
#include <dirent.h>
#include <initializer_list>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <fcntl.h>

#if defined(__LP64__)
using Pointer = long;
#define MEMORY_64
#else
using Pointer = unsigned int;
#define MEMORY_32
#endif

// Unsigned base types.
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

// Signed base types.
typedef signed char int8;
typedef signed short int int16;
typedef signed int int32;
typedef signed long long int64;

using Dword = int;
using Float = float;

struct value_t {
    std::vector<char> bytes;
    int offset = 0;
};

extern jmp_buf env;
/**
 * 注入内存读写类
 * 注1: 读到空指针会导致SIGSEGV, 请监听SIGSEGV信号并
 * 使用 siglongjmp sigsetjmp 防止程序因为读空指针而崩溃
 */
class InjectMemory {
private:
    pid_t target_pid;
    size_t pointer_size;
    size_t page_size;
public:

    InjectMemory() : pointer_size(sizeof(long)) {};

    /**
     * 进程pid初始化
     * @param process_pid 目标进程pid
     */
    bool init(int process_pid);

    /**
     * 进程名初始化
     * @param process_name 目标进程名
     */
    bool init(const char *process_name);

    /**
     * 判断地址是否是内存陷阱 (默认仅在 readPointer 中调用)
     * @param address 地址
     * @return bool
     */
    bool isMemoryTrap(Pointer _addr, int _size);

    /**
     * 获取模块基地址
     * @param module_name 模块名
     * @return 地址
     */
    Pointer getModuleBase(const char *module_name);

    /**
     * 获取模块尾地址
     * @param module_name 模块名
     * @return 地址
     */
    Pointer getModuleEnd(const char *module_name);

    /**
     * 读指针
     * @param address 地址
     * @return 结果
     */
    Pointer readPointer(Pointer address);

    /**
     * 读Dword
     * @param address 地址
     * @return 结果
     */

    Pointer readPointers(long address, std::initializer_list<Pointer> offset);

    Dword readDword(Pointer address);

    /**
     * 读Float
     * @param address 地址
     * @return 结果
     */
    Float readFloat(Pointer address);

    /**
     * 内存读取
     * @tparam T 读取类型
     * @param address 地址
     * @return 读取结果
     */
    template<typename T>
    T read(Pointer address);

    /**
     * 内存写入
     * @tparam T 写入类型
     * @param address 地址
     * @param data 要写入的内容
     */
    template<typename T>
    void write(Pointer address, T data);

    /**
     * 读数组 (使用完请 delete[] 否则会内存泄露)
     * @tparam T
     * @param address 地址
     * @param size 最大读取长度
     * @return 结果数组
     */
    template<typename T>
    T *readArr(Pointer address, ssize_t size);

    /**
     * 读字符串 (使用完请 delete[] 否则会内存泄露)
     * @param address 地址
     * @param max_size 最大读取长度
     * @param real_size 字符串真实长度
     * @return 结果字符串
     */
    char *readStr(Pointer address, ssize_t max_size, ssize_t *real_size);
};

template<typename T>
T InjectMemory::read(Pointer address) {
    T data;
    bzero(&data, sizeof(T));
    if (pointer_size == 4){
        if (abs((int)address) < 0xFFFFFF || abs((int)address) > 0xFFFFFFFF) {
            return 0;
        }
    }else{
        if (abs((long long)address) < 0xFFFFFF || abs((long long)address) > 0xFFFFFFFFFFFFFFFF) {
            return 0;
        }
    }
    if(isMemoryTrap(address, sizeof(T))){
        return 0;
    }
    int r = sigsetjmp(env, 1);
    if (r == 0) {
        memmove(&data, (void *) address, sizeof(T));
    }
    return data;
}


template<typename T>
void InjectMemory::write(Pointer address, T data) {
    memmove((void *) address, &data, sizeof(T));
}


template<typename T>
T *InjectMemory::readArr(Pointer address, ssize_t size) {
    T *ptr = new T[size];
    memmove(ptr, (void *) address, (sizeof(T) * size));
    return ptr;
}

jmp_buf env;

void handler(int signum) {
    siglongjmp(env, 1);
}

bool InjectMemory::init(int process_pid) {
    page_size = sysconf(_SC_PAGESIZE);
    signal(SIGSEGV, handler);
    return true;
}

bool InjectMemory::init(const char *process_name) {
    page_size = sysconf(_SC_PAGESIZE);
    signal(SIGSEGV, handler);
    return true;
}

Pointer InjectMemory::getModuleBase(const char *module_name) {
    char *phgsr;
    char jjjj_N[64];
    long startaddr = 0;
    char line[1024];
    bool bssOF = false, LastIsSo = false;
    strcpy(jjjj_N, module_name);
    phgsr = strtok(jjjj_N, ":");
    module_name = phgsr;
    phgsr = strtok(NULL, ":");
    if (phgsr) {
        if (strcmp(phgsr, "bss") == 0) {bssOF = true;}
    }
    FILE *p = fopen("/proc/self/maps", "r");
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

Pointer InjectMemory::getModuleEnd(const char *module_name) {
    FILE *fp;
    Pointer temp = 0, addr = 0;
    char filename[32] = "/proc/self/maps", buffer[1024];
    fp = fopen(filename, "rt");
    if (fp != nullptr) {
        if (pointer_size == 4) { //32bit
            int temp32 = 0, addr32 = 0;
            while (fgets(buffer, sizeof(buffer), fp)) {
                if (strstr(buffer, module_name)) {
                    sscanf(buffer, "%x-%x %*s", &temp32, &addr32);
                    break;
                }
            }
            temp = temp32;
            addr = addr32;
        }
#ifdef MEMORY_64
        else { //64bit
            while (fgets(buffer, sizeof(buffer), fp)) {
                if (strstr(buffer, module_name)) {
                    sscanf(buffer, "%llx-%llx %*s", &temp, &addr);
                    break;
                }
            }
        }
#endif
        fclose(fp);
    }
    return addr;
}

Pointer InjectMemory::readPointer(Pointer address) {
    return read<Pointer>(address);
}


Dword InjectMemory::readDword(Pointer address) {
    return read<Dword>(address);
}


Float InjectMemory::readFloat(Pointer address) {
    return read<Float>(address);
}

Pointer InjectMemory::readPointers(Pointer address, std::initializer_list<Pointer> offset) {
    for (Pointer Offset : offset)
    {
        address = readPointer(address) + Offset;
    }
    return address;
}


char *InjectMemory::readStr(Pointer address, ssize_t max_size, ssize_t *real_size) {
    char *data = new char[max_size];
    for (int i = 0; i < max_size; i++) {
        data[i] = read<char>(address + (sizeof(char) * i));
        if (data[i] == 0x0) {
            if (real_size) {
                *real_size = i;
            }
            break;
        }
    }
    return data;
}

bool InjectMemory::isMemoryTrap(Pointer _addr, int _size) {
    Pointer address = _addr & (~(page_size - 1));
    unsigned int size = _addr - address + _size;
    size % page_size ? size += page_size - size % page_size : size;
    unsigned int mincore_size = (size + page_size - 1) / page_size;
    unsigned char *mincore_buffer = (unsigned char *) malloc(mincore_size);
    if (syscall(__NR_mincore, (void *) address, size, mincore_buffer) != 0) { // syscall failed
        free(mincore_buffer);
        return false;
    }
    for (int32_t i = 0; i < mincore_size; i++) {
        if ((mincore_buffer[i] & 1) != 1)
            return true;
    }
    free(mincore_buffer);
    return false;
}