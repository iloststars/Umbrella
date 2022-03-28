//
// Created by ilost on 2021/7/6.
//

#ifndef 蜻蜓_MEMORY_H
#define 蜻蜓_MEMORY_H
#include <jni.h>
#include <string>
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


#if defined(__arm__)
#define process_vm_readv_syscall 376
#define process_vm_writev_syscall 377
#elif defined(__aarch64__)
#define process_vm_readv_syscall 270
#define process_vm_writev_syscall 271
#elif defined(__i386__)
#define process_vm_readv_syscall 347
#define process_vm_writev_syscall 348
#elif defined(__x86_64__)
#define process_vm_readv_syscall 310
#define process_vm_writev_syscall 311
#else
#error "Unsupported architecture"
#endif
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef signed char int8;
typedef signed short int int16;
typedef signed int int32;
typedef signed long long int64;

using Dword = int;
using Float = float;
using Pointer = long;

class Memory {
private:
    pid_t target_pid;
    size_t pointer_size;
    size_t page_size;
public:
    Memory() : pointer_size(sizeof(long)) {};

    //进程pid
    bool init(int process_pid);

    //进程名
    bool init(const char *process_name);

    /**
     * 判断地址是否是内存陷阱 (默认仅在 readPointer 中调用)
     * @param address 地址
     * @return bool
     */
    bool isMemoryTrap(Pointer address);

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
    Dword readDword(Pointer address);

    /**
     * 读Float
     * @param address 地址
     * @return 结果
     */
    Float readFloat(Pointer address);

    Pointer readPointers(Pointer address, std::initializer_list<Pointer> offset);

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
    //读数组
    template<typename T>
    T *readArr(Pointer address, ssize_t size);

    //读取字符串(使用后请delete)
    char *readStr(Pointer address/*地址*/, ssize_t max_size/*最大读取长度*/, ssize_t *real_size/*字符串真实长度*/);
};


template<typename T>
T Memory::read(Pointer address) {
    if (address == 0 || isMemoryTrap(address)){
        printf("内存陷阱 %ld",address);
        return 0;
    }
    T data;
    bzero(&data, sizeof(T));
    if (address < 0xFFFFFF) {
        return data;
    }
    memmove(&data, (void *) address, sizeof(T));
    return data;
}


bool Memory::init(int process_pid) {
    page_size = sysconf(_SC_PAGESIZE);
    return true;
}

bool Memory::init(const char *process_name) {
    page_size = sysconf(_SC_PAGESIZE);
    return true;
}

Pointer Memory::getModuleBase(const char *module_name) {
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

Pointer Memory::getModuleEnd(const char *module_name) {
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
        } else { //64bit
            while (fgets(buffer, sizeof(buffer), fp)) {
                if (strstr(buffer, module_name)) {
                    sscanf(buffer, "%lx-%lx %*s", &temp, &addr);
                    break;
                }
            }
        }
        fclose(fp);
    }
    return addr;
}


Pointer Memory::readPointer(Pointer address) {
    return read<Pointer>(address);
}


Dword Memory::readDword(Pointer address) {
    return read<Dword>(address);
}


Float Memory::readFloat(Pointer address) {
    return read<Float>(address);
}

Pointer Memory::readPointers(long address, std::initializer_list<Pointer> offset) {
    for (Pointer Offset : offset)
    {
        address = readPointer(address) + Offset;
    }
    return address;
}

char *Memory::readStr(Pointer address, ssize_t max_size, ssize_t *real_size) {
    char *data = new char[max_size];
    for (int i = 0; i < max_size; i++) {
        data[i] = read<char>(address + (sizeof(char) * i));
        if (data[i] == 0x0) {
            if (real_size){
                *real_size = i;
            }
            break;
        }
    }
    return data;
}

bool Memory::isMemoryTrap(Pointer address) {
    unsigned char vec = 0;
    Pointer pageStart = ((Pointer) address) & (~(page_size - 1u));
    syscall(__NR_mincore, (void *) pageStart, page_size, &vec);
    return vec != 1;
}



#endif //蜻蜓_MEMORY_H
